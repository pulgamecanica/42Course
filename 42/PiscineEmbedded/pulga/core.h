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
