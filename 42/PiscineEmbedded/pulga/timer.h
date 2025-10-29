#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <stdint.h>
#include "core.h"
#include "pins.h"

// ───────────────
// Timer identifiers
// ───────────────
typedef enum {
  TIMER1
  // Extend later: TIMER0, TIMER2
} Timer;

// ───────────────
// Timer modes
// ───────────────
typedef enum {
  TIMER1_MODE_NORMAL,
  TIMER1_MODE_CTC,
  TIMER1_MODE_FAST_PWM_ICR1
} TimerMode;

// ───────────────
// Prescaler values
// ───────────────
typedef enum {
  NO_CLOCK = 0,
  CLK_1 = 1,
  CLK_8 = 8,
  CLK_64 = 64,
  CLK_256 = 256,
  CLK_1024 = 1024
} TimerPrescaler;

// ───────────────
// Timer1 specific functions
// ───────────────

// Page 140 from RFTM (TCCR1A) & Page 141/142 from RTFM (TCCR1B)
static inline void timer1_set_mode(TimerMode mode) {
  TCCR1B = 0;
  TCCR1A = 0;
  switch (mode) {
    case TIMER1_MODE_NORMAL:
      TCCR1B = 0;
      break;
    case TIMER1_MODE_CTC:
      _SET(TCCR1B, WGM12); // Page 141 (4th row in table)
      break;
    case TIMER1_MODE_FAST_PWM_ICR1: // Page 142 (14th row in table) // Why not using OCR1A? Because we need it for duty cycle control
      _SET(TCCR1A, WGM11);
      _SET(TCCR1B, WGM12);
      _SET(TCCR1B, WGM13);
      break;
  }
}

// WGM(Waveform Generation Mode)
static inline TimerMode timer1_get_mode(void) {
  uint8_t wgm = ((TCCR1B & ((1 << WGM13) | (1 << WGM12))) >> 2) | // We shift 2 to the left so it's  4bit
                (TCCR1A & ((1 << WGM11) | (1 << WGM10)));
  switch (wgm) {
    case 0: return TIMER1_MODE_NORMAL;
    case 4 /*0b0100*/: return TIMER1_MODE_CTC;
    case 14 /*0b1110*/: return TIMER1_MODE_FAST_PWM_ICR1;
    default:     return TIMER1_MODE_NORMAL;
  }
}

// Page 142/143 from RTFM (TCCR1B)
static inline void timer1_set_prescaler(TimerPrescaler prescaler) {
 /*
  * TCCR1B &= ... =
  * _CLEAR(TCCR1B, CS12);
  * _CLEAR(TCCR1B, CS11);
  * _CLEAR(TCCR1B, CS10);
  * it just takes less operations to do it this way
  */
  TCCR1B &= ~((1 << CS12)|(1 << CS11)|(1 << CS10)); // clear prescaler bits
  switch (prescaler) {
    case NO_CLOCK: break;
    case CLK_1:    _SET(TCCR1B, CS10); break;
    case CLK_8:    _SET(TCCR1B, CS11); break;
    case CLK_64:   _SET(TCCR1B, CS11); _SET(TCCR1B, CS10); break;
    case CLK_256:  _SET(TCCR1B, CS12); break;
    case CLK_1024: _SET(TCCR1B, CS12); _SET(TCCR1B, CS10); break;
  }
}

static inline TimerPrescaler timer1_get_prescaler(void) {
  uint8_t mask = TCCR1B & ((1 << CS12)|(1 << CS11)|(1 << CS10));
  switch (mask) {
    case 0: return NO_CLOCK;
    case 1 << CS10: return CLK_1;
    case 1 << CS11: return CLK_8;
    case (1 << CS11)|(1 << CS10): return CLK_64;
    case 1 << CS12: return CLK_256;
    case (1 << CS12)|(1 << CS10): return CLK_1024;
  }
  return NO_CLOCK;
}

// Page 92 from RTFM (PB1 is OC1A) & Page 140 for table mapping
static inline void timer1_enable_oc1a_toggle(void) {
    _SET(TCCR1A, COM1A0);
}

static inline void timer1_disable_oc1a_toggle(void) {
    _CLEAR(TCCR1A, COM1A0);
}

// Page 140
static inline void timer1_enable_oc1a_non_inverting(void) {
  _SET(TCCR1A, COM1A1);
  _CLEAR(TCCR1A, COM1A0);
}

static inline void timer1_disable_oc1a_non_inverting(void) {
  _CLEAR(TCCR1A, COM1A1);
  _CLEAR(TCCR1A, COM1A0);
}

// Page 134 (Formula -> 1st formula)
// PWM(Pulse Width Modulation) frequency via TOP value (ICR1)
static inline void timer1_set_pwm_frequency(uint32_t freq_hz) {
  uint16_t prescaler = timer1_get_prescaler();
  if (prescaler == 0 || freq_hz == 0) return;

  ICR1 = (16000000 / (prescaler * freq_hz)) - 1;
}

static inline void timer1_set_pwm_period_ms(uint32_t period_ms) {
  timer1_set_pwm_frequency(1000UL / period_ms);
}

static inline void timer1_set_pwm_period_us(uint32_t period_us) {
  timer1_set_pwm_frequency(1000000UL / period_us);
}

// Page 122 from RTFM
// Duty cycle as a percentage [0–100]
// if you let duty_percent = 0..100%, then:
// The proportion of time the pin must be HIGH = duty_percent / 100.0.
// That corresponds to the counter running from 0 up to OCR1A (out of ICR1 total counts).
// So OCR1A / ICR1 = duty_percent / 100.
//                 duty_percent​
// OCR1A = ICR1 * -------------- = OCR1A = ICR1 * %[0..1]
//                      100
static inline void timer1_set_duty_cycle(uint8_t duty_percent) {
  if (duty_percent > 100) duty_percent = 100;
  OCR1A = (uint16_t)((double)(ICR1 * duty_percent) / 100);
}

// Page 132 (Formula)
// We can use this formula if we know the frequency which we want to use
static inline void timer1_set_toggle_frequency(uint32_t freq_hz) {
  uint16_t prescaler = timer1_get_prescaler();
  if (prescaler == 0  || freq_hz == 0) return ;

  uint32_t top = ((double)F_CPU / (double)(2UL * prescaler * freq_hz)) - 1;
  OCR1A = (uint16_t)top;
}

// If you know the period in microseconds
static inline void timer1_set_toggle_period_ms(uint32_t period_ms) {
  if (period_ms == 0) return;
  timer1_set_toggle_frequency(1000UL / period_ms);
}

// If you know the period in miliseconds
static inline void timer1_set_toggle_period_us(uint32_t period_us) {
  if (period_us == 0) return;
  timer1_set_toggle_frequency(1000000UL / period_us);
}

// This function automatically detects the Timer1 mode and applies the correct formula
static inline void timer1_set_frquency_auto(uint32_t freq_hz) {
  TimerMode tm = timer1_get_mode();
  switch (tm) {
    case TIMER1_MODE_NORMAL:
      // TODO: I didn't use this mode so far so I also haven't done this
      break;
    case TIMER1_MODE_CTC:
      timer1_set_toggle_frequency(freq_hz);
      break;
    case TIMER1_MODE_FAST_PWM_ICR1:
      timer1_set_pwm_frequency(freq_hz);
      break;
  }
}

static inline void timer1_set_period_ms_auto(uint32_t period_ms) {
  if (period_ms == 0) return;
  timer1_set_frquency_auto(1000L / period_ms);
}

static inline void timer1_set_period_us_auto(uint32_t period_us) {
  if (period_us == 0) return;
  timer1_set_frquency_auto(1000000L / period_us);

}

#endif // TIMER_H

