#include "pulga/core.h"
#include "pulga/pins.h"
#include "pulga/timer.h"

/* Page 108 and 109
 * Diff between Fast PWM and Phase Correct PWM
 *
 * Fast PWM: counter goes 0 → TOP → 0 → TOP (fast wraparound)
 * Phase Correct PWM: counter goes 0 → TOP → 0 → TOP,
 * but reverses direction each time, doubling the period.
 *
 */

#define LED PIN_B1


int main(void) {
    setPinMode(LED, OUTPUT);                    // OC1A = PB1
    timer1_set_mode(TIMER1_MODE_FAST_PWM_ICR1); // Set to CTC (TCCR1B => WGM12) see func. for refs.
    timer1_set_prescaler(CLK_1024);             // Set prescaler to 1024 (TCCR1B => CS12 & CS10)

    timer1_set_pwm_frequency(1);  // ICR1 = 15624
    timer1_set_duty_cycle(10);      // See page 122 (OCR1A => ICR1 * %[0..1])

    // Configure OC1A output behavior: non-inverting mode - Page 140 Table 16-2 row 3
    // The counter counts from 0 up to TOP (which is ICR1 in that PWM mode).
    // In Fast PWM, COM1A1 = 1 and COM1A0 = 0 → clear on compare, set at bottom
    // The output logic uses OCR1A: when the counter matches OCR1A,
    // the output pin switches (e.g. from HIGH to LOW in non-inverting mode)
    timer1_enable_oc1a_non_inverting();

    while (1);
}

/*
int main(void)
{
  // Set PB1 as output
  DDRB |= (1 << PB1);

  // Fast PWM, TOP = ICR1 (mode 14: WGM13:0 = 1110)
  TCCR1A = (1 << WGM11);
  TCCR1B = (1 << WGM12) | (1 << WGM13);

  // Non-inverting mode on OC1A (Clear on compare, set at BOTTOM)
  TCCR1A |= (1 << COM1A1);

  // Set TOP for 1 Hz
  ICR1 = 15624;

  // Set duty cycle to 10 %
  OCR1A = 1562;

  // Prescaler = 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
  while(42) ;
  return 0;
}
*/
