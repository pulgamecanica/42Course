/*
 * Subject:
 * You must write a program that turns on the LED D1 (PB0).
 * You must use only AVR registers (DDRX, PORTX, PINX).
 */

#include "pulga/core.h"
#include "pulga/pins.h"

int main(void) {
  setPinMode(PIN_B0, OUTPUT);
  setPinMode(PIN_D2, INPUT);
  writePin(PIN_B0, LED_OFF);
  while(42) {
    if (readPin(PIN_D2) == BUTTON_PRESSED)
      writePin(PIN_B0, LED_ON);
    else
      writePin(PIN_B0, LED_OFF);
  }
  return 0;
}
