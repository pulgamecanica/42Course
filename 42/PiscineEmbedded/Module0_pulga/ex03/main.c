/*
 * Subject:
 * You must write a program that reverses the state of the PB0 LED each time the
   SW1 button (PD2) changes from the released state to the pressed state.
 * You must use only AVR registers ( DDRX, PORTX, PINX ).
 */

#include "pulga/core.h"
#include "pulga/pins.h"
#include <util/delay.h>

int main(void) {
  unsigned was_pressed = 0;

  setPinMode(PIN_B0, OUTPUT);
  setPinMode(PIN_D2, INPUT);
  while(42) {
    _delay_ms(42);
    if (readPin(PIN_D2) == BUTTON_RELEASED) // Detect when button D2 is released
      was_pressed = 0;
    if (!was_pressed && readPin(PIN_D2) == BUTTON_PRESSED) { // Detect if the button D2 is released
      was_pressed = 1;
      togglePin(PIN_B0);
    }
  }
  return 0;
}
