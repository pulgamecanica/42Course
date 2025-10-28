#ifndef PINS_H
#define PINS_H

#include "core.h"
#include <avr/io.h>

// ───────────────
// Types
// ───────────────
typedef enum {
    PIN_B0, PIN_B1, PIN_B2, PIN_B3, PIN_B4, PIN_B5, PIN_B6, PIN_B7,
    PIN_C0, PIN_C1, PIN_C2, PIN_C3, PIN_C4, PIN_C5,
    PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_D4, PIN_D5, PIN_D6, PIN_D7
} Pin;

typedef enum { INPUT, OUTPUT } PinMode;
typedef enum { LOW = 0, HIGH = 1 } PinState;

#define LED_ON          ((PinState)HIGH)
#define LED_OFF         ((PinState)LOW)
#define BUTTON_PRESSED  ((PinState)LOW)
#define BUTTON_RELEASED ((PinState)HIGH)

// ───────────────
// Pin mapping tables
// ───────────────
static const char PIN_PORTS[] = {
    // PORTB
    'B','B','B','B','B','B','B','B',
    // PORTC
    'C','C','C','C','C','C',
    // PORTD
    'D','D','D','D','D','D','D','D'
};

static const uint8_t PIN_BITS[] = {
    // PORTB
    0,1,2,3,4,5,6,7,
    // PORTC
    0,1,2,3,4,5,
    // PORTD
    0,1,2,3,4,5,6,7
};

// ───────────────
// Register pointer tables
// ───────────────
static volatile uint8_t * const DDR_REGS[]  = { &DDRB, &DDRC, &DDRD };
static volatile uint8_t * const PORT_REGS[] = { &PORTB, &PORTC, &PORTD };
static volatile uint8_t * const PIN_REGS[]  = { &PINB, &PINC, &PIND };

// ───────────────
// Helper macros
// ───────────────
#define PORT_INDEX(p) ((PIN_PORTS[p]) - 'B')
#define PIN_DDR(p)    (*DDR_REGS[PORT_INDEX(p)])
#define PIN_PORT(p)   (*PORT_REGS[PORT_INDEX(p)])
#define PIN_PIN(p)    (*PIN_REGS[PORT_INDEX(p)])

// ───────────────
// Inline functions
// ───────────────
static inline void setPinMode(Pin pin, PinMode mode) {
    if (mode == OUTPUT) _SET(PIN_DDR(pin), PIN_BITS[pin]);
    else                _CLEAR(PIN_DDR(pin), PIN_BITS[pin]);
}

static inline void writePin(Pin pin, PinState state) {
    if (state == HIGH) _SET(PIN_PORT(pin), PIN_BITS[pin]);
    else               _CLEAR(PIN_PORT(pin), PIN_BITS[pin]);
}

static inline void togglePin(Pin pin) {
    _SET(PIN_PIN(pin), PIN_BITS[pin]);
}

static inline PinState readPin(Pin pin) {
    return _IS_SET(PIN_PIN(pin), PIN_BITS[pin]) ? HIGH : LOW;
}

#endif // PINS_H
