#include "pulga/core.h"
#include "pulga/pins.h"
#include "pulga/timer.h"
#include <util/delay.h>

#define LED PIN_B1
#define BUTTON_DEC PIN_D4  // SW2
#define BUTTON_INC PIN_D2  // SW1

const Pin BUTTON_PINS[4] = { BUTTON_DEC, BUTTON_INC };

void setup() {
  for (int i = 0; i < 2; ++i)
    setPinMode(BUTTON_PINS[i], INPUT);
  setPinMode(LED, OUTPUT);
  timer1_set_mode(TIMER1_MODE_FAST_PWM_ICR1); // Set to CTC (TCCR1B => WGM12) see func. for refs.
  timer1_set_prescaler(CLK_1024);             // Set prescaler to 1024 (TCCR1B => CS12 & CS10)

  timer1_set_pwm_frequency(100);        // ICR1 = 15624
  timer1_set_duty_cycle(10);          // See page 122 (OCR1A => ICR1 * %[0..1])
  timer1_enable_oc1a_non_inverting(); // COM1A1 => OC1A set non inverting mode
}

int main(void) {
  setup();
  int dc = 10; // duty cycle

  while (1) {
    // Check increment button
    if (readPin(BUTTON_INC) == BUTTON_PRESSED) {
      dc += 10;
      if (dc > 100)
        dc = 100;
      timer1_set_duty_cycle(dc);
      // Wait for release
      while (readPin(BUTTON_INC) == BUTTON_PRESSED)
        _delay_ms(10);
    }

    // Check decrement button
    if (readPin(BUTTON_DEC) == BUTTON_PRESSED) {
      dc -= 10;
      if (dc < 10)
        dc = 10;
      timer1_set_duty_cycle(dc);
      // Wait for release
      while (readPin(BUTTON_DEC) == BUTTON_PRESSED)
        _delay_ms(10);
    }
  }
  return 0;
}

