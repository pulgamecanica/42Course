```c
#ifndef PINS_H
#define PINS_H

#include "core.h"

// ───────────────
// Arrays mapping enum to port number and bit
// ───────────────
static const char PIN_PORTS[] = {
    'A','A','A','A','A','A',         // PORTA
    'B','B','B','B','B','B','B','B', // PORTB
    'C','C','C','C','C','C',         // PORTC
    'D','D','D','D','D','D','D','D'  // PORTD
};

static const uint8_t PIN_BITS[] = {
    0,1,2,3,4,5,          // PORTA
    0,1,2,3,4,5,6,7,      // PORTB
    0,1,2,3,4,5,          // PORTC
    0,1,2,3,4,5,6,7       // PORTD
};

// ───────────────
// Types
// ───────────────
typedef enum { PIN_A0, PIN_A1, PIN_A2, PIN_A3, PIN_A4, PIN_A5,
               PIN_B0, PIN_B1, PIN_B2, PIN_B3, PIN_B4, PIN_B5, PIN_B6, PIN_B7,
               PIN_C0, PIN_C1, PIN_C2, PIN_C3, PIN_C4, PIN_C5,
               PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_D4, PIN_D5, PIN_D6, PIN_D7 } Pin;

#define PIN_DDR(p)   _CAT3(DDR, PIN_PORTS[p], )
#define PIN_PORT(p)  _CAT3(PORT, PIN_PORTS[p], )
#define PIN_PIN(p)   _CAT3(PIN, PIN_PORTS[p], )

typedef enum { INPUT, OUTPUT } PinMode;
typedef enum { LOW=0, HIGH=1 } PinState;

// ───────────────
// Inline functions
// ───────────────
static inline void setPinMode(Pin pin, PinMode mode) {
    if(mode == OUTPUT) _SET(PIN_DDR(pin), PIN_BITS[pin]);
    else               _CLEAR(PIN_DDR(pin), PIN_BITS[pin]);
}

static inline void writePin(Pin pin, PinState state) {
    if(state == HIGH) _SET(PIN_PORT(pin), PIN_BITS[pin]);
    else              _CLEAR(PIN_PORT(pin), PIN_BITS[pin]);
}

/*
When you write to the PINx register on an AVR (for example, PIND or PINB),
any bit that is written with a 1 will toggle the corresponding output bit in PORTx.
*/
static inline void togglePin(Pin pin) {
    _SET(PIN_PIN(pin), PIN_BITS[pin]);
}

static inline PinState readPin(Pin pin) {
    return _IS_SET(PIN_PIN(pin), PIN_BITS[pin]) ? HIGH : LOW;
}

#endif // PINS_H
```

---

<details>
<summary><code>setPinMode(Pin pin, PinMode mode)</code></summary>
<br>
    
```c
static inline void setPinMode(Pin pin, PinMode mode) {
    if(mode == OUTPUT) _SET(PIN_DDR(pin), PIN_BITS[pin]);
    else               _CLEAR(PIN_DDR(pin), PIN_BITS[pin]);
}
```

### Description

Configures a pin as input or output.

### Example

```c
setPinMode(PIN_B1, OUTPUT); // Configure PB1 as output
setPinMode(PIN_D2, INPUT);  // Configure PD2 as input
```

### Notes

* Expands to register operations using `_CAT3(DDR, X, )`.
* Doesn’t use any Arduino library; pure register manipulation.

</details>

---

<details>
<summary><code>writePin(Pin pin, PinState state)</code></summary>
<br>
    
```c
static inline void writePin(Pin pin, PinState state) {
    if(state == HIGH) _SET(PIN_PORT(pin), PIN_BITS[pin]);
    else              _CLEAR(PIN_PORT(pin), PIN_BITS[pin]);
}
```

### Description

Writes `HIGH` or `LOW` to a pin.

### Example

```c
writePin(PIN_B1, HIGH); // Turn on LED connected to PB1
writePin(PIN_B1, LOW);  // Turn off LED
```

</details>

---

<details>
<summary><code>togglePin(Pin pin)</code></summary>
<br>
    
```c
static inline void togglePin(Pin pin) {
    _SET(PIN_PIN(pin), PIN_BITS[pin]);
}
```

### Description

Toggles the output state of a pin by writing `1` to the corresponding `PINx` register bit.

### Example

```c
togglePin(PIN_B1); // Toggles PB1
```

### Notes

This takes advantage of the AVR’s special “write-1-to-toggle” behavior.

</details>

---

<details>
<summary><code>readPin(Pin pin)</code></summary>
<br>
    
```c
static inline PinState readPin(Pin pin) {
    return _IS_SET(PIN_PIN(pin), PIN_BITS[pin]) ? HIGH : LOW;
}
```

### Description

Reads the digital state (`HIGH` or `LOW`) of a pin.

### Example

```c
if (readPin(PIN_D2) == HIGH) {
    // Input is high
}
```

</details>

---

## Summary

`pins.h` provides a **hardware-agnostic** way to manipulate pins without using Arduino’s functions.
It builds on `core.h` macros to:

* Dynamically reference registers (`DDRx`, `PORTx`, `PINx`) using `_CAT3`.
* Operate at compile time (no runtime penalty).
* Remain clean and explicit for learning and debugging.
