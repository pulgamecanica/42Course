/*
 * This library is part of my learning jorney at 42Paris
 *
 * I am able to abstract everything I learn and compule it into a small API.
 * I stronly believe anyone doing the piscine should do something similar.
*/
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

/*
# [BIT]

_BIT(b): Generates a magic number all zeros and the *"one"* bit we want to be on "1"

```
_BIT(0) 0x0000 0001 : 1
_BIT(1) 0x0000 0010 : 2
_BIT(2) 0x0000 0100 : 4
_BIT(3) 0x0000 1000 : 8
_BIT(b) 0x000? 0000 : 2**b
```

It's actually also the formula for `2**b`
*/

/*
# [SET]

_SET(reg,x): Sets the bit position in the desired register "1"

If we had:

```
PORTB => 0x0001 1100
```

And we do:

```
_SET(PORTB, 0); => 0x0001 1101
```

Or if we had done:
```
_SET(PORTB, 1); => 0x0001 1110
```

*/












