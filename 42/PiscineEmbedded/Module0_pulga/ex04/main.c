#include "pulga/core.h"
#include "pulga/pins.h"
#include <util/delay.h>

// ───────────────
// LED pins
// ───────────────
#define LED0 PIN_B0
#define LED1 PIN_B1
#define LED2 PIN_B2
#define LED3 PIN_B4

// ───────────────
// Button pins
// ───────────────
#define BUTTON_DEC PIN_D4  // SW1
#define BUTTON_INC PIN_D2  // SW2

const Pin LED_PINS[4] = { LED0, LED1, LED2, LED3 };

// ───────────────
// Display a 4-bit value on LEDs
// ───────────────
static inline void display_number(uint8_t num) {
  for (int i = 0; i < 4; ++i) {
    // Map bit i of num to LED i
    if (num & (1 << i))
      writePin(LED_PINS[i], LED_ON);
    else
      writePin(LED_PINS[i], LED_OFF);
  }
}

int main(void) {
  unsigned num = 0;

  // Set LED pins as outputs
  for (int i = 0; i < 4; ++i)
    setPinMode(LED_PINS[i], OUTPUT);

  // Set button pins as inputs
  setPinMode(BUTTON_INC, INPUT);
  setPinMode(BUTTON_DEC, INPUT);

  display_number(num);

  while (1) {
    _delay_ms(50);

  // Check increment button
  if (readPin(BUTTON_INC) == BUTTON_PRESSED) {
    ++num;
    display_number(num);
    // Wait for release
    while (readPin(BUTTON_INC) == BUTTON_PRESSED)
      _delay_ms(10);
  }

  // Check decrement button
  if (readPin(BUTTON_DEC) == BUTTON_PRESSED) {
    --num;
    display_number(num);
    // Wait for release
    while (readPin(BUTTON_DEC) == BUTTON_PRESSED)
      _delay_ms(10);
    }
  }
}
