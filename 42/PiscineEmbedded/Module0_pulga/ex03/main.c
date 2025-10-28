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
#define PORT_OFF(x,bit)    PORT##x &= (~(1<<PORT##x##bit)) // <- set the reg bit to (1)
#define PORT_TOGGLE(x,bit) PIN##x |= (1<<PIN##x##bit)    // <- toggle the reg bit

// USES PIN BY DEFAULT
#define IS_ON(x,bit)  PIN##x & (1<<PIN##x##bit) // <- check if the bit is on
#define IS_OFF(x,bit) !(IS_ON(x,bit))


int main(void) {
  unsigned was_pressed = 0;
	MODE_OUT(B, 0);
  MODE_IN(D, 2);
	while(42) {
    _delay_ms(42);
    if (IS_ON(D, 2))
      was_pressed = 0;
    if (!was_pressed && IS_OFF(D, 2)) {
      was_pressed = 1;
      PORT_TOGGLE(B, 0);
    }
  }
	return 0;
}
