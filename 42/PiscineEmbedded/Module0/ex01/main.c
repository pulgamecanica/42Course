/*
 * Subject:
 * You must write a program that turns on the LED D1 (PB0).
 * You must use only AVR registers (DDRX, PORTX, PINX).
 */

#include <avr/io.h>

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

// https://ww1.microchip.com/downloads/en/Appnotes/AVR1000b-Getting-Started-Writing-C-Code-for-AVR-DS90003262B.pdf - page 11
// #define PORTMUX (*(PORTMUX_t *) 0x0200) /* Port Multiplexer */
// #define PORTA (*(PORT_t *) 0x0400) /* I/O Ports */
// #define PORTB (*(PORT_t *) 0x0420) /* I/O Ports */
// #define PORTC (*(PORT_t *) 0x0440) /* I/O Ports */

// DDR = Data Direction Register
#define MODE_OUT(x,bit) DDR##x  |= (1<<DD##x##bit)   // <- set the mode to output
#define PORT_ON(x,bit)  PORT##x |= (1<<PORT##x##bit)  // <- set the reg bit to (1)

int main(void) {
	MODE_OUT(B, 0);
	PORT_ON(B, 0);
	while(42) /* Marvin the paranoid android cringe sounds */;
	return 0;
}
