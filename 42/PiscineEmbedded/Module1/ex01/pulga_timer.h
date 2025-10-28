#ifndef PULGA_UTILS_TIMER_H
#define PULGA_UTILS_TIMER_H

#include <avr/io.h>

//
// ---- TIMER1 MODES ----
//

#define TIMER_16_NORMAL   0
#define TIMER_16_CTC      (1 << WGM12)
#define TIMER_16_FAST_PWM ((1 << WGM12) | (1 << WGM13))

// Usage: TIMER_16_SET_MODE(CTC)
#define TIMER_16_SET_MODE(mode) \
    do { \
        TCCR1A = 0; \
        TCCR1B = (mode); \
    } while (0)

//
// ---- COMPARE OUTPUT BEHAVIOR ----
//

#define TIMER_16_COMA_TOGGLE (1 << COM1A0)
#define TIMER_16_COMA_CLEAR  (1 << COM1A1)
#define TIMER_16_COMA_SET    ((1 << COM1A1) | (1 << COM1A0))

// Usage: TIMER_16_SET_COMPARE_TOGGLE(A)
#define TIMER_16_SET_COMPARE_TOGGLE(chan) \
    do { \
        if (chan == 'A') TCCR1A |= (1 << COM1A0); \
        else if (chan == 'B') TCCR1A |= (1 << COM1B0); \
    } while (0)

//
// ---- PRESCALER SETTINGS ----
//

#define TIMER_16_PRESCALE_NONE   (1)
#define TIMER_16_PRESCALE_8      (2)
#define TIMER_16_PRESCALE_64     (3)
#define TIMER_16_PRESCALE_256    (4)
#define TIMER_16_PRESCALE_1024   (5)

// Usage: TIMER_16_SET_PRESCALER(1024)
#define TIMER_16_SET_PRESCALER(div) \
    do { \
        TCCR1B = (TCCR1B & ~0x07) | \
                 ((div) == 1 ? TIMER_16_PRESCALE_NONE : \
                 (div) == 8 ? TIMER_16_PRESCALE_8 : \
                 (div) == 64 ? TIMER_16_PRESCALE_64 : \
                 (div) == 256 ? TIMER_16_PRESCALE_256 : \
                 (div) == 1024 ? TIMER_16_PRESCALE_1024 : 0); \
    } while (0)

//
// ---- OCR HELPERS ----
//

// Usage: TIMER_16_SET_OCR(A, 7812)
#define TIMER_16_SET_OCR(chan, val) \
    do { \
        if (chan == 'A') OCR1A = (val); \
        else if (chan == 'B') OCR1B = (val); \
    } while (0)

#endif // endif PULGA_UTILS_TIMER_H
