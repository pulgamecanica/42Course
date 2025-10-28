#ifndef PULGA_UTILS_H
#define PULGA_UTILS_H

#include "pulga_types.h"
#include <avr/io.h>

// DDR = Data Direction Register
// MODE USES DDR BY DEFAULT, will implement generic and API with defaults (see ex04)
#define TO_OUTPUT(x,bit) DDR##x  |= (1<<DD##x##bit)    // <- set the mode to output
#define TO_INPUT(x,bit)  DDR##x  &= (~(1<<DD##x##bit)) // <- set the mode to output

#define LED_ON(x,bit)     PORT##x |= (1<<PORT##x##bit)    // <- set the reg bit to (1)
#define LED_OFF(x,bit)    PORT##x &= (~(1<<PORT##x##bit)) // <- set the reg bit to (0)
#define LED_TOGGLE(x,bit) PIN##x |= (1<<PIN##x##bit)    // <- toggle the reg bit

// USES PIN BY DEFAULT
#define IS_ON(x,bit)  PIN##x & (1<<PIN##x##bit) // <- check if the bit is on
#define IS_OFF(x,bit) !(IS_ON(x,bit))


#endif
