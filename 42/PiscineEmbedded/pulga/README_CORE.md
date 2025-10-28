```c
#ifndef CORE_H
#define CORE_H

#include <avr/io.h>

// ───────────────
// CPU frequency
// ───────────────
#ifndef F_CPU
# define F_CPU 16000000UL
#endif

// ───────────────
// Bit helpers
// ───────────────
#define _BIT(b)       (1 << (b))
#define _SET(reg,b)   ((reg) |= _BIT(b))
#define _CLEAR(reg,b) ((reg) &= ~_BIT(b))
#define _TOGGLE(reg,b)((reg) ^= _BIT(b))
#define _IS_SET(reg,b)((reg) & _BIT(b))

// ───────────────
// Token concatenation
// ───────────────
#define _CAT(a,b)     a##b
#define _CAT3(a,b,c)  a##b##c

#endif
```

---

<details>
<summary><code>_BIT(b)</code></summary>
<br>
    
```c
#define _BIT(b)       (1 << (b))
```

### Description

Creates a bitmask by shifting `1` to the left by `b` positions.

### Example

```c
uint8_t mask = _BIT(3); // 0b00001000
```

### Notes

Useful for building masks without magic numbers.
Equivalent to `(1 << b)`.

</details>

---

<details>
<summary><code>_SET(reg, b)</code></summary>
<br>
    
```c
#define _SET(reg,b)   ((reg) |= _BIT(b))
```

### Description

Sets bit `b` in register `reg` to `1`.

### Example

```c
_SET(DDRB, 1); // Sets bit 1 of DDRB
```

### Notes

Directly manipulates hardware registers.
Expands to `(reg |= (1 << b))`.

</details>

---

<details>
<summary><code>_CLEAR(reg, b)</code></summary>
<br>
    
```c
#define _CLEAR(reg,b) ((reg) &= ~_BIT(b))
```

### Description

Clears bit `b` in register `reg` (sets it to `0`).

### Example

```c
_CLEAR(PORTD, 5); // Clears bit 5 of PORTD
```

</details>

---

<details>
<summary><code>_TOGGLE(reg, b)</code></summary>
<br>
    
```c
#define _TOGGLE(reg,b)((reg) ^= _BIT(b))
```

### Description

Flips bit `b` in register `reg`.

### Example

```c
_TOGGLE(PORTB, 0); // Toggles bit 0 of PORTB
```

</details>

---

<details>
<summary><code>_IS_SET(reg, b)</code></summary>
<br>
    
```c
#define _IS_SET(reg,b)((reg) & _BIT(b))
```
### Description

Checks whether bit `b` is set in register `reg`.

### Example

```c
if (_IS_SET(PIND, 2)) {
    // Pin D2 is HIGH
}
```

</details>

---

<details>
<summary><code>_CAT(a, b)</code> and <code>_CAT3(a, b, c)</code></summary>
<br>
    
### Description

Concatenates tokens at compile time.

### Example

```c
#define PORT(x) _CAT(PORT, x)
#define DDR(x)  _CAT(DDR, x)
#define PIN(x)  _CAT(PIN, x)

uint8_t x = PORT(B); // Expands to PORTB
```

`_CAT3(a, b, c)` allows concatenating three tokens (e.g. `PORT`, `B`, and `_MASK` → `PORTB_MASK`).

### Notes

This is the backbone of dynamic register access in your macros.

</details>

---

## Summary

`core.h` defines **foundational macros** used across all other modules in Pulga:

* Universal bit operations
* Compile-time register concatenation
* CPU frequency definition

Every other header (like `pins.h`, `timer.h`, etc.) depends on these.
