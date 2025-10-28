/*
 * Subject:
 * You must write a program that turns on the LED D1 (PB0).
 * You must use only AVR registers (DDRX, PORTX, PINX).
 */

#include "pulga/core.h"
#include "pulga/pins.h"

int main(void) {
  setPinMode(PIN_B0, OUTPUT);
  writePin(PIN_B0, LED_ON);
	while(42) /* Marvin the paranoid android cringe sounds */;
	return 0;
}
