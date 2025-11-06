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
#define PRESCALER CLK_64
#define LED_PB1  PIN_B1  // LED on PB1 (controlled by Timer1 OC1A)
#define PWM_STEPS 100U
#define FADE_PERIOD_MS 1000.0  // full 0..100..0 cycle duration
#define OVERFLOW_TIME_S  (256.0 * PRESCALER / (double)F_CPU)
#define UPDATE_TIME_S    (FADE_PERIOD_MS / 1000.0 / (PWM_STEPS * 2))
#define OVERFLOWS_PER_UPDATE ((uint16_t)(UPDATE_TIME_S / OVERFLOW_TIME_S + 0.5))

// Global variables for duty cycle control
volatile uint8_t duty_cycle = 0;
volatile uint8_t direction = 0;  // 0 = increasing, 1 = decreasing
volatile uint8_t overflow_counter = 0;

// 1. Overflow period formula
// Toverflow=256×prescaler/FCPU (timer0 is 8 bits, so 256 ticks per overflow)
//
// Timer0 overflow callback
// Timer0 with prescaler 64 overflows every ~1.024ms
// We count 5 overflows to get approximately 5ms between updates
// This gives us 200 updates per second (perfect for 200 steps in 1 second)
void on_timer0_overflow(void) {
  static const uint16_t overflows_per_update = OVERFLOWS_PER_UPDATE;
  overflow_counter++;

  // Update duty cycle every 5 overflows (~5ms)
  if (overflow_counter >= overflows_per_update) {
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

int main(void) {
  // Configure PB1 as OUTPUT for PWM
  setPinMode(LED_PB1, OUTPUT);

  // Configure Timer1 for Fast PWM on OC1A (PB1)
  // Set Timer1 to Fast PWM mode (using ICR1 as TOP)
  timer1_set_mode(TIMER_MODE_FAST_PWM);

  // Set prescaler to 64
  // Timer1 will tick at: 16MHz / 64 = 250kHz
  timer1_set_prescaler(PRESCALER);

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

  // Configure Timer0 for periodic interrupts
  timer0_init_overflow_interrupt(PRESCALER);

  // Enable periodic overflow interrupt
  timer0_enable_overflow_interrupt();

  // Attach callback for Timer0 overflow
  timer0_attach_overflow(on_timer0_overflow);

  // Enable Timer0 overflow interrupt
  timer0_enable_overflow_interrupt();

  // Enable global interrupts
  interrupts_enable();

	// Main loop does nothing - everything is handled by Timer0 ISR
  while (42) ;
  // The LED brightness will smoothly vary from 0% to 100% and back
  // in exactly 1 second (200 steps * 5ms = 1000ms)

  return 0;
}
