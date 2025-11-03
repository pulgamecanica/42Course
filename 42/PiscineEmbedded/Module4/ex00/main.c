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
#include <util/delay.h>

// Hardware definitions
#define LED_D1  PIN_B0  // LED D1 on PB0
#define SW1     PIN_D2  // Button SW1 on PD2 (INT0)

// Interrupt Service Routine callback
// This function is called when INT0 is triggered (button pressed)
void on_button_press(void) {
	togglePin(LED_D1);
}

int main(void) {
	// Configure LED D1 as OUTPUT
	setPinMode(LED_D1, OUTPUT);
	writePin(LED_D1, LED_OFF);

	// Configure SW1 as INPUT with pull-up resistor
	setPinMode(SW1, INPUT);
	writePin(SW1, HIGH);  // Enable internal pull-up

	// Configure INT0 to trigger on falling edge (button press)
	int0_set_mode(INT0_FALLING_EDGE);

	// Attach our callback function to INT0
	attach_int0(on_button_press);

	// Enable INT0 interrupt
	int0_enable();

	// Enable global interrupts
	interrupts_enable();

	// Main loop does nothing - everything is handled by interrupts
	while (42) ;

	return 0;
}
