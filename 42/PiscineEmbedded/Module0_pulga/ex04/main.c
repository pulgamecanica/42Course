/*
 * Subject:
 * You must write a program that reverses the state of the PB0 LED each time the
   SW1 button (PD2) changes from the released state to the pressed state.
 * You must use only AVR registers ( DDRX, PORTX, PINX ).
 */

#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

// DDR = Data Direction Register
// MODE USES DDR BY DEFAULT, will implement generic and API with defaults (see ex04)
#define MODE_OUT(x,bit) DDR##x  |= (1<<DD##x##bit)    // <- set the mode to output
#define MODE_IN(x,bit)  DDR##x  &= (~(1<<DD##x##bit)) // <- set the mode to output

#define PORT_ON(x,bit)     PORT##x |= (1<<PORT##x##bit)    // <- set the reg bit to (1)
#define PORT_OFF(x,bit)    PORT##x &= (~(1<<PORT##x##bit)) // <- set the reg bit to (0)
#define PORT_TOGGLE(x,bit) PIN##x |= (1<<PIN##x##bit)    // <- toggle the reg bit

// USES PIN BY DEFAULT
#define IS_ON(x,bit)  PIN##x & (1<<PIN##x##bit) // <- check if the bit is on
#define IS_OFF(x,bit) !(IS_ON(x,bit))

void display_number(unsigned num)
{
    // We only care about 4 bits (PB0, PB1, PB2, PB4)
    uint8_t mask = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
    uint8_t pattern = 0;

    // Reduce num to 4 bits
    num &= 0x0F;

    // Map the lower 4 bits of num to PB0, PB1, PB2, PB4
    if (num & (1 << 0)) pattern |= (1 << PB0);
    if (num & (1 << 1)) pattern |= (1 << PB1);
    if (num & (1 << 2)) pattern |= (1 << PB2);
    if (num & (1 << 3)) pattern |= (1 << PB4);

    // Clear the LED bits, then set the new pattern
    PORTB = (PORTB & ~mask) | pattern;
}

int main(void) {
  unsigned num = 0;
	MODE_OUT(B, 0);
  MODE_OUT(B, 1);
  MODE_OUT(B, 2);
  MODE_OUT(B, 4);
  MODE_IN(D, 4);
  MODE_IN(D, 2);
	while(42) {
    _delay_ms(42);
    unsigned should_add = IS_OFF(D, 2);
    unsigned should_decrement = IS_OFF(D, 4);

    if (!should_add && !should_decrement) continue;

    if (should_add) {
      display_number(++num);
      _delay_ms(42);
      while(IS_OFF(D,2)) {_delay_ms(42);} // Wait for release
    }
    if (should_decrement) {
      display_number(--num);
      _delay_ms(42);
      while(IS_OFF(D,4)) {_delay_ms(42);} // Wait for release
    }
  }
	return 0;
}
