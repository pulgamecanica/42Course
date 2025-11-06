/*
 * Module4 - ex00
 * Subject:
 * You are required to write a program that changes the state of LED D1 when
 * the button SW1 is pressed.
 * You must use interrupts to read the state of the button. Reading the PINx
 * registers is not allowed.
 */

#include "pulga/pins.h"
#include "pulga/interrupt.h"
#include "pulga/timer.h"
#include <util/delay.h>

// Hardware definitions
#define LED_D1  PIN_B0  // LED D1 on PB0
#define SW1     PIN_D2  // Button SW1 on PD2 (INT0)

// Shared state (must be volatile)
volatile uint8_t debounce_active = 0;

// INT0 ISR: Button pressed
void on_button_press(void) {
  if (!debounce_active) {
    debounce_active = 1;
  }
}

// ─────────────────────────────
// Timer1 ISR: runs every 20 ms
// ─────────────────────────────
// https://www.avrfreaks.net/s/topic/a5C3l000000UWEpEAO/t137717?comment=P-1483338
void on_timer_tick(void) {
  static uint8_t debounce_counter = 0;

  if (debounce_active) {
    debounce_counter++;

    if (debounce_counter >= 1) {  // 1 * 20 ms = 20 ms debounce window
      debounce_counter = 0;
      debounce_active = 0;

      // Confirm button is still pressed (LOW because pull-up)
      if (readPin(SW1) == LOW) {
        togglePin(LED_D1);
      }
    }
  }
}

int main(void) {
	// Configure LED D1 as OUTPUT
	setPinMode(LED_D1, OUTPUT);
	writePin(LED_D1, LED_OFF);

	// Configure SW1 as INPUT with pull-up resistor
	setPinMode(SW1, INPUT);
	writePin(SW1, HIGH);  // Enable internal pull-up

  // Configure INT0
  int0_set_mode(FALLING_EDGE);
  attach_int0(on_button_press);
  int0_enable();

  // Configure Timer1 for CTC interrupt every 20 ms
  timer1_init_ctc_mode(20);
  timer1_attach_compare_a(on_timer_tick);
  timer1_enable_compare_a_interrupt();

	// Enable global interrupts
	interrupts_enable();

	// Main loop does nothing - everything is handled by interrupts
	while (42) ;

	return 0;
}
