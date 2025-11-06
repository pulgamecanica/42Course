#ifndef ADC_H
#define ADC_H

/*
 * adc.h — Simple, safe and flexible ADC abstraction for ATmega328P
 *
 * Features:
 *  - safe defaults: AVCC, 8-bit left-adjust, prescaler 128
 *  - blocking single-shot read
 *  - non-blocking sequence reads using ADC interrupt and user callback
 *  - automatic ADC shutdown when sequence completes (unless continuous)
 *  - ability to query/set currently selected channel/ref/prescaler
 *  - per-header static state (header-only)
 *
 * Usage examples:
 *
 *   // Default init
 *   adc_init_default();
 *   uint8_t v = adc_read_blocking_8(ADC_CH0);
 *
 *   // Non-blocking multi-channel: reads channels 0,1,2 once and disables ADC afterwards.
 *   uint8_t channels[] = { ADC_CH0, ADC_CH1, ADC_CH2 };
 *   adc_request_sequence(channels, 3, adc_seq_cb, NULL, false);
 *
 *   // Callback prototype:
 *   //   void adc_seq_cb(uint16_t value, uint8_t channel, void *ctx);
 *
 * Notes:
 *  - ADC_TEMP is offered but internal sensors may require special handling/calibration.
 *  - This header uses <avr/interrupt.h> and the ADC_vect ISR to implement non-blocking mode.
 */

#include "core.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"

// Reference voltage options - Page 257 Table 24.3
#define ADC_REF_AREF   0x00u                       /* AREF, external cap on AREF pin */
#define ADC_REF_AVCC   (_BIT(REFS0))              /* AVcc with external capacitor at AREF pin */
#define ADC_REF_INT11  (_BIT(REFS1) | _BIT(REFS0)) /* Internal 1.1 V reference */

// Prescaler options (ADPS[2:0]) - Page 259 Table 24.5
#define ADC_PRESC_2    (0x01) // 0 also works
#define ADC_PRESC_4    (_BIT(ADPS1))
#define ADC_PRESC_8    (_BIT(ADPS0) | _BIT(ADPS1))
#define ADC_PRESC_16   (_BIT(ADPS2))
#define ADC_PRESC_32   (_BIT(ADPS0) | _BIT(ADPS2))
#define ADC_PRESC_64   (_BIT(ADPS1) | _BIT(ADPS2))
#define ADC_PRESC_128  (_BIT(ADPS0) | _BIT(ADPS1) | _BIT(ADPS2))

// ADC Channels (ATmega328P) - Page 258 Table 24.4
#define ADC_CH0  0x00u   /* ADC0 (PC0) */
#define ADC_CH1  0x01u
#define ADC_CH2  0x02u
#define ADC_CH3  0x03u
#define ADC_CH4  0x04u
#define ADC_CH5  0x05u
#define ADC_CH6  0x06u
#define ADC_CH7  0x07u
#define ADC_TEMP 0x08u   /* Internal temperature sensor (implementation note below) */

/* ------------------------
 * Callback type used for non-blocking reads
 *   value  - measured ADC value (8 or 10 bits depending on left-adjust)
 *   channel - channel that produced the value
 *   ctx    - user context pointer passed through
 * ------------------------ */
typedef void (*adc_callback_t)(uint16_t value, uint8_t channel, void *ctx);

// Internal state (static within header)
static struct {
    bool initialized;
    uint8_t ref;            /* ADMUX REFS bits, this is related to the voltage reference for conversions */
    uint8_t prescaler;      /* ADCSRA ADPS bits, will affect the # of samples and cycles */
    bool left_adjust;       /* ADLAR true => 8-bit reads from ADCH */
    uint8_t current_channel;
    /* Sequence state for non-blocking reads */
    const uint8_t *seq_channels;
    uint8_t seq_len;
    uint8_t seq_idx;
    bool seq_continuous;     /* if true, ADC stays on and repeats sequence */
    adc_callback_t seq_cb;
    void *seq_ctx;
} __adc_state = {
    .initialized = false,
    .ref = ADC_REF_AVCC,
    .prescaler = ADC_PRESC_128,
    .left_adjust = true,
    .current_channel = ADC_CH0,
    .seq_channels = 0,
    .seq_len = 0,
    .seq_idx = 0,
    .seq_continuous = false,
    .seq_cb = 0,
    .seq_ctx = 0
};

// Low-level accessors

// Enable ADC peripheral (does not change ISR flags)
static inline void adc_enable(void) {
    ADCSRA |= _BIT(ADEN); // Page 149 - "The prescaler starts counting from the moment the..."
    __adc_state.initialized = true;
}

// Disable ADC peripheral (also disables ADC interrupt)
static inline void adc_disable(void) {
    ADCSRA &= ~(_BIT(ADEN); // Page 149
    ADCSRA &= ~(_BIT(ADIE); // disable ADC interrupt - Page 259 (Bit 4 – ADIF: ADC Interrupt Flag)
    __adc_state.initialized = false;
}

// Set reference voltage bits (use ADC_REF_* macros) - Page 257 Table 24-3
static inline void adc_set_reference(uint8_t ref_bits) {
    // Page 257 REFS1:REFS0 are bits 7:6 of ADMUX
    ADMUX = (ADMUX & 0x3F) | (ref_bits & 0xC0);
    __adc_state.ref = ref_bits & 0xC0; // C0: 1100 0000 &  0011 1111
}

// Get the current reference bits
static inline uint8_t adc_get_reference(void) {
    return __adc_state.ref;
}

// Set left adjust (non-zero enables ADLAR -> 8-bit reads from ADCH)
static inline void adc_set_left_adjust(uint8_t enable) {
    if (enable) {
        ADMUX |= (_BIT(ADLAR);
        __adc_state.left_adjust = true;
    } else {
        ADMUX &= ~(_BIT(ADLAR);
        __adc_state.left_adjust = false;
    }
}

// Query left-adjust state
static inline bool adc_is_left_adjust(void) {
    return __adc_state.left_adjust;
}

// Set ADC prescaler (ADPS2:0)
static inline void adc_set_prescaler(uint8_t presc) {
    ADCSRA = (ADCSRA & 0xF8) | (presc & 0x07);
    __adc_state.prescaler = presc & 0x07;
}

// Get ADC prescaler
static inline uint8_t adc_get_prescaler(void) {
    return __adc_state.prescaler;
}

// Set the active ADC channel (0..7). For ADC_TEMP use ADC_TEMP constant.
static inline void adc_select_channel(uint8_t ch) {
    if (ch <= 7) {
        /* Clear MUX bits [3:0] and set */
        ADMUX = (ADMUX & 0xF0) | (ch & 0x0F);
        __adc_state.current_channel = ch;
    } else if (ch == ADC_TEMP) {
        /* Some devices need special mux / reference settings to access internal sensors.
           We set the MUX bits as requested (0x08) and keep REFS as-is. */
        ADMUX = (ADMUX & 0xF0) | (ADC_TEMP & 0x0F);
        __adc_state.current_channel = ADC_TEMP;
    } else {
        /* invalid channel — ignore (defensive) */
    }
}

// Return last selected channel
static inline uint8_t adc_get_selected_channel(void) {
    return __adc_state.current_channel;
}

// Start a single conversion (non-blocking). Use adc_is_complete()/adc_read() to get value or use ISR.
static inline void adc_start_conversion(void) {
    ADCSRA |= _BIT(ADSC);
}

// Test whether conversion finished (ADIF set)
static inline bool adc_is_complete(void) {
    return (ADCSRA & _BIT(ADIF)) != 0;
}

// Clear conversion flag (write 1 to ADIF to clear)
static inline void adc_clear_flag(void) {
    ADCSRA |= _BIT(ADIF);
}

/* Read ADC result:
 *  - if left-adjusted: returns 8-bit value (0..255) from ADCH
 *  - otherwise: returns 10-bit value (0..1023) from ADC register
 */
static inline uint16_t adc_read_raw(void) {
    if (__adc_state.left_adjust) {
        return (uint16_t)ADCH; /* ADCH holds the high 8 bits in left-adjust mode */
    } else {
        return (uint16_t)ADC;  /* ADC (ADCL + ADCH) holds 10-bit result (ADCL must be read first by hardware) */
    }
}

/* ------------------------
 * Blocking convenience helpers
 * ------------------------ */

/* Initialize ADC with defaults:
 *  - enable ADC
 *  - AVCC reference
 *  - left-adjust (8-bit)
 *  - channel 0
 *  - prescaler 128
 */
static inline void adc_init_default(void) {
    adc_disable(); /* ensure clean state */
    adc_set_reference(ADC_REF_AVCC);
    adc_set_left_adjust(1);
    adc_select_channel(ADC_CH0);
    adc_set_prescaler(ADC_PRESC_128);
    adc_enable();
}

/* Blocking single-shot read of a channel. Returns 8-bit value if left-adjust set, else 10-bit value. */
static inline uint16_t adc_read_blocking(uint8_t ch) {
    adc_select_channel(ch);
    adc_enable();
    adc_clear_flag();
    adc_start_conversion();
    /* Wait for conversion to finish */
    while (!adc_is_complete()) { /* busy wait */ }
    uint16_t v = adc_read_raw();
    adc_clear_flag();
    /* By default we turn ADC off after blocking read to save power. If you prefer to keep ADC enabled,
       call adc_enable() before. */
    adc_disable();
    return v;
}

/* Convenience returning 8-bit (clamped) */
static inline uint8_t adc_read_blocking_8(uint8_t ch) {
    uint16_t v = adc_read_blocking(ch);
    if (v > 0xFF) return 0xFF;
    return (uint8_t)v;
}

/* ------------------------
 * Non-blocking sequence API (ISR-driven)
 *
 * adc_request_sequence(ch_array, len, cb, ctx, continuous)
 *
 * - ch_array: pointer to an array of channel ids (values 0..7 or ADC_TEMP)
 * - len: number of channels in array (must be > 0)
 * - cb: callback called for each sample: cb(value, channel, ctx)
 * - ctx: opaque user pointer passed to callback
 * - continuous: if false ADC will automatically disable after one full sequence; if true it will repeat forever
 *
 * Behavior:
 *  - ADC interrupt ADIE is enabled and ADC_vect is used.
 *  - each conversion result calls cb; after last channel:
 *     - if continuous: seq_idx resets to 0 and sequence repeats
 *     - else: ADC interrupt disabled and ADC turned off
 * ------------------------ */

/* Request non-blocking sequence. Returns true on success, false on invalid args. */
static inline bool adc_request_sequence(const uint8_t *channels, uint8_t len, adc_callback_t cb, void *ctx, bool continuous) {
    if (channels == NULL || len == 0 || cb == NULL) return false;

    /* Basic validation of channels (only allow 0..7 or ADC_TEMP) */
    for (uint8_t i = 0; i < len; ++i) {
        uint8_t ch = channels[i];
        if (!(ch <= 7 || ch == ADC_TEMP)) return false;
    }

    /* store state */
    __adc_state.seq_channels = channels;
    __adc_state.seq_len = len;
    __adc_state.seq_idx = 0;
    __adc_state.seq_continuous = continuous;
    __adc_state.seq_cb = cb;
    __adc_state.seq_ctx = ctx;

    /* Setup ADC for the first channel and start conversions with interrupts */
    adc_enable();
    adc_select_channel(__adc_state.seq_channels[0]);
    adc_clear_flag();
    ADCSRA |= (1 << ADIE); /* enable ADC interrupt */
    adc_start_conversion();

    return true;
}

/* Cancel active sequence (if any). This disables ADC interrupt and turns ADC off. */
static inline void adc_cancel_sequence(void) {
    ADCSRA &= ~(1 << ADIE);
    __adc_state.seq_channels = 0;
    __adc_state.seq_len = 0;
    __adc_state.seq_idx = 0;
    __adc_state.seq_cb = 0;
    __adc_state.seq_ctx = 0;
    __adc_state.seq_continuous = false;
    adc_disable();
}

/* Attach / detach a plain ISR callback (alternative to sequence API).
 * This registers a callback that will be called for every conversion when ADIE is enabled
 * and a conversion completes. If you use adc_request_sequence, it will set/restore seq_cb itself.
 */
static adc_callback_t __adc_user_callback = 0;
static void *__adc_user_ctx = 0;

static inline void adc_attach_callback(adc_callback_t cb, void *ctx) {
    __adc_user_callback = cb;
    __adc_user_ctx = ctx;
}

static inline void adc_detach_callback(void) {
    __adc_user_callback = 0;
    __adc_user_ctx = 0;
}

/* ------------------------
 * ADC ISR — handles sequence progression and user callbacks
 * ------------------------ */
ISR(ADC_vect) {
    uint16_t value = adc_read_raw();
    uint8_t ch = __adc_state.current_channel;

    /* Clear ADIF by writing 1 (also safe to do it immediately) */
    ADCSRA |= (1 << ADIF);

    /* Call sequence callback if present */
    if (__adc_state.seq_cb) {
        __adc_state.seq_cb(value, ch, __adc_state.seq_ctx);

        /* advance to next index */
        __adc_state.seq_idx++;
        if (__adc_state.seq_idx >= __adc_state.seq_len) {
            if (__adc_state.seq_continuous) {
                __adc_state.seq_idx = 0;
            } else {
                /* sequence finished — disable ADIE and optionally disable ADC */
                ADCSRA &= ~(1 << ADIE);
                adc_disable();
                return;
            }
        }

        /* select next channel and trigger next conversion */
        uint8_t next_ch = __adc_state.seq_channels[__adc_state.seq_idx];
        adc_select_channel(next_ch);
        adc_start_conversion();
        return;
    }

    /* If there's a plain user callback attached (not sequence), call it */
    if (__adc_user_callback) {
        __adc_user_callback(value, ch, __adc_user_ctx);
    }
    /* If neither a sequence nor a user callback is set, just disable ADIE to avoid repeated IRQs */
    else {
        ADCSRA &= ~(1 << ADIE);
        adc_disable();
    }
}

/* ------------------------
 * Convenience power-saving helpers
 * ------------------------ */

/* Turn ADC on but clear ADIF/ADSC/ADIE to be in a clean state */
static inline void adc_power_on_clean(void) {
    ADCSRA &= ~((1 << ADSC) | (1 << ADIF) | (1 << ADIE));
    adc_enable();
}

/* ------------------------
 * Implementation notes / cautions
 * ------------------------
 *  - Internal temperature sensor (ADC_TEMP) often requires selecting the bandgap
 *    input and specific reference — check your datasheet. This header will select MUX=0x08
 *    when you pass ADC_TEMP, but calibration and proper reference selection is left to you.
 *
 *  - The non-blocking sequence API uses ADC_vect and will disable/enable the ADC
 *    automatically per the 'continuous' argument.
 *
 *  - Blocking reads disable the ADC at the end to save power. If you want to
 *    perform many reads quickly, call adc_enable() once and then adc_read_blocking()
 *    repeatedly (or use the sequence API).
 *
 *  - This header is defensive and clamps/validates input in many places; if you
 *    want less defensive behavior for performance, say so and I can provide a
 *    faster variant with fewer checks.
 *
 *  - If you want conversions triggered from a timer or external trigger, we can
 *    add functions to enable ADC Auto Triggering and set the trigger source.
 *
 */

#endif /* ADC_H */
