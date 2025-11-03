/*
 * Module4 - ex01
 * Subject:
 * Configure Timer0 to trigger a periodic interrupt that varies the duty cycle
 * of the LED PB1 controlled by Timer1.
 * - The frequency of Timer1 must be high enough to no longer see the LED blinking.
 * - The duty cycle should vary in a loop from 0% to 100% and then from 100% to 0%
 *   in 1 second.
 */

#include "pulga/pins.h"
#include "pulga/timer.h"
#include "pulga/interrupt.h"
#include <avr/io.h>

// Hardware definitions
#define LED_PB1  PIN_B1  // LED on PB1 (controlled by Timer1 OC1A)

// Global variables for duty cycle control
volatile uint8_t duty_cycle = 0;
volatile uint8_t direction = 0;  // 0 = increasing, 1 = decreasing
volatile uint8_t overflow_counter = 0;

// Timer0 overflow callback
// Timer0 with prescaler 64 overflows every ~1.024ms
// We count 5 overflows to get approximately 5ms between updates
// This gives us 200 updates per second (perfect for 200 steps in 1 second)
void on_timer0_overflow(void) {
	overflow_counter++;

	// Update duty cycle every 5 overflows (~5ms)
	if (overflow_counter >= 5) {
		overflow_counter = 0;

		// Update duty cycle based on direction
		if (direction == 0) {
			// Increasing: 0% -> 100%
			duty_cycle++;
			if (duty_cycle >= 100) {
				duty_cycle = 100;
				direction = 1;  // Switch to decreasing
			}
		} else {
			// Decreasing: 100% -> 0%
			duty_cycle--;
			if (duty_cycle == 0) {
				direction = 0;  // Switch to increasing
			}
		}

		// Update Timer1 PWM duty cycle
		timer1_set_duty_cycle(duty_cycle);
	}
}

// Initialize Timer0 for periodic interrupts
void timer0_init_overflow_interrupt(void) {
	// Set Timer0 to Normal mode (overflow mode)
	TCCR0A = 0;
	TCCR0B = 0;

	// Set prescaler to 64 (CS01 | CS00)
	// Timer0 will tick at: F_CPU / 64 = 16MHz / 64 = 250kHz
	// Overflow period: 256 / 250kHz = 1.024ms
	TCCR0B |= _BIT(CS01) | _BIT(CS00);

	// Enable Timer0 overflow interrupt
	TIMSK0 |= _BIT(TOIE0);

	// Initialize counter
	TCNT0 = 0;
}

int main(void) {
	// Configure PB1 as OUTPUT for PWM
	setPinMode(LED_PB1, OUTPUT);

	// ─────────────────────────────────────────────────────────
	// Configure Timer1 for Fast PWM on OC1A (PB1)
	// ─────────────────────────────────────────────────────────

	// Set Timer1 to Fast PWM mode (using ICR1 as TOP)
	timer1_set_mode(TIMER_MODE_FAST_PWM);

	// Set prescaler to 64
	// Timer1 will tick at: 16MHz / 64 = 250kHz
	timer1_set_prescaler(CLK_64);

	// Set PWM frequency to 1kHz (high enough to avoid visible flickering)
	// Formula: ICR1 = (F_CPU / (prescaler * freq)) - 1
	//         ICR1 = (16MHz / (64 * 1000Hz)) - 1 = 249
	timer1_set_pwm_frequency(1000);

	// Enable non-inverting PWM on OC1A (PB1)
	// When OCR1A < TCNT1: pin is LOW
	// When OCR1A >= TCNT1: pin is HIGH
	timer1_enable_oc1a_non_inverting();

	// Set initial duty cycle to 0%
	timer1_set_duty_cycle(0);

	// ─────────────────────────────────────────────────────────
	// Configure Timer0 for periodic interrupts
	// ─────────────────────────────────────────────────────────

	timer0_init_overflow_interrupt();

	// Attach callback for Timer0 overflow
	timer0_attach_overflow(on_timer0_overflow);

	// Enable Timer0 overflow interrupt
	timer0_enable_overflow_interrupt();

	// Enable global interrupts
	interrupts_enable();

	// Main loop does nothing - everything is handled by Timer0 ISR
	while (42) {
		// The LED brightness will smoothly vary from 0% to 100% and back
		// in exactly 1 second (200 steps * 5ms = 1000ms)
	}

	return 0;
}
