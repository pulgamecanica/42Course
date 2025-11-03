/*
 * Module4 - ex02
 * Subject:
 * - SW1 button increments a value
 * - SW2 button decrements a value
 * - Display the value on LEDs D1, D2, D3, D4 in binary
 * - Use interrupts with nothing in main loop
 *
 * Challenge: LEDs are on PB0, PB1, PB2, PB4 (not PB3!)
 * Requires bit manipulation to map counter bits correctly.
 */

#include "pulga/pins.h"
#include "pulga/interrupt.h"
#include <avr/io.h>

// Hardware definitions - LEDs
#define LED_D1  PIN_B0  // LED D1 on PB0 (bit 0 of counter)
#define LED_D2  PIN_B1  // LED D2 on PB1 (bit 1 of counter)
#define LED_D3  PIN_B2  // LED D3 on PB2 (bit 2 of counter)
#define LED_D4  PIN_B4  // LED D4 on PB4 (bit 3 of counter) <- Gap at PB3!
#define RGB_BLUE PIN_B3 // RGB LED D5 blue channel on PB3 (avoid activating it!)

// Hardware definitions - Buttons
// NOTE: Check your hardware! You need buttons on both PD2 (INT0) and PD3 (INT1)
// If you only have SW1(PD2) and SW1(PD4), you'll need to wire a button to PD3
#define SW1     PIN_D2  // Button SW1 on PD2 (INT0) - increment
#define SW2     PIN_D3  // Button SW2 on PD3 (INT1) - decrement

// LED pin array mapping (handles non-contiguous GPIO)
const Pin LED_PINS[4] = { LED_D1, LED_D2, LED_D3, LED_D4 };

// Global counter variable (0-15, 4 bits)
volatile uint8_t counter = 0;

// Display the counter value on the 4 LEDs
// Uses the LED_PINS array to handle non-contiguous GPIO mapping
void display_counter(void) {
	for (int i = 0; i < 4; i++) {
		// Map bit i of counter to LED i
		if (counter & (1 << i))
			writePin(LED_PINS[i], LED_ON);
		else
			writePin(LED_PINS[i], LED_OFF);
	}
}

// SW1 interrupt callback - increment counter
void on_sw1_press(void) {
	counter = (counter + 1) & 0x0F;  // Increment and wrap at 15
	display_counter();

	// Clear interrupt flag to handle debouncing
	// Any bounces that occurred during display_counter() are ignored
	EIFR |= (1 << INTF0);
}

// SW2 interrupt callback - decrement counter
void on_sw2_press(void) {
	counter = (counter - 1) & 0x0F;  // Decrement and wrap at 0
	display_counter();

	// Clear interrupt flag to handle debouncing
	EIFR |= (1 << INTF1);
}

int main(void) {
	// ─────────────────────────────────────────────────────────
	// Configure LEDs as OUTPUT
	// ─────────────────────────────────────────────────────────
	for (int i = 0; i < 4; i++)
		setPinMode(LED_PINS[i], OUTPUT);

	// Make sure RGB blue (PB3) is OFF
	setPinMode(RGB_BLUE, OUTPUT);
	writePin(RGB_BLUE, LOW);

	// ─────────────────────────────────────────────────────────
	// Configure buttons as INPUT with pull-ups
	// ─────────────────────────────────────────────────────────
	setPinMode(SW1, INPUT);
	writePin(SW1, HIGH);  // Enable internal pull-up

	setPinMode(SW2, INPUT);
	writePin(SW2, HIGH);  // Enable internal pull-up

	// ─────────────────────────────────────────────────────────
	// Configure INT0 for SW1 (increment)
	// ─────────────────────────────────────────────────────────
	int0_set_mode(INT0_FALLING_EDGE);  // Trigger on button press
	attach_int0(on_sw1_press);
	int0_enable();

	// ─────────────────────────────────────────────────────────
	// Configure INT1 for SW2 (decrement)
	// ─────────────────────────────────────────────────────────
	int1_set_mode(INT1_FALLING_EDGE);  // Trigger on button press
	attach_int1(on_sw2_press);
	int1_enable();

	// Display initial value (0)
	display_counter();

	// Enable global interrupts
	interrupts_enable();

	// Main loop does nothing - everything is handled by interrupts
	while (42) {
		// Empty loop as required by the exercise
	}

	return 0;
}
