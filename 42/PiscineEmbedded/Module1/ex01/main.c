// Timer Normal mode Registers: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf#page=108
// Overflow Flag:  https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf#page=95

/*
 * 16MegaHertz = 16Million Hz = 16 000 000 Hz
 * This means that we have 16 million times per second
 *
 * Prescaler & Predivisor
 * - Prescaler:
 *   The Prescaler allows you to control further the frequency.
 *   1 => Nothing happens, same 16MHz
 *   4 => 16 / 4 = 4, then 4MHz
 * - Predivisor:
 *   The Predivisor is the division ration of the timer, similar to Prescaler
 *   1 => Nothing
 *   32 => 16 / 32 => 0.5 = 500000
 * Both combined:
 * f:16MHz
 * P1:4
 * P2:32
 * Then: 16M/4/32 = 125000Hz = 125kHz => 125 000 per second
 *
 * On the ATmega238P the Prescaler is: CLKPR (0x61) Page 46 from RTFM
 * Prescaler: the value you put in CLKPR bits will be the POWER used 2**CLKPR (See page 47 from RTFM)
 * `To avoid unintentional changes of clock frequency, a special write procedure must be followed to change the CLKPS bits` - page 45 from RTFM
 *
 * What is CTC? its Clear Timer on Compare!
 * What is Toggle OC1A/B? It's toggling the OC1A/B on Compare! (PB1 is OC1A on ATMega38B) - Page 92 from RTFM
 *
 * How to find the prescaler to get 1Hz? - Page 132 from RTFM
 *
 *                 F_CPU
 * f(toggle) = -------------
 *             2*N*(1+OCR1A)
 *
 * N = prescaler
 * f(toggle) = 1Hz (period = 1/1 = 1)
 * Then:
 *
 *             F_CPU
 * OCR1A = ------------- - 1
 *         2*N*f(toggle)
 *
 * Then we are free to choose any prescaler we want - Page 117 from RTFM
 * We can choose for example: 1024 or 256
 * Let N = 1024
 *           16e6           16e6
 * OCR1A = --------- - 1 = ------ - 1 =  7812.5 - 1 = 7811.5
 *         2*1024*1         2048
 *
 * Let N = 256
 *           16e6
 * OCR1A = -------- - 1  = 62500 - 1 = 62499
 *         2*256*1
 */

// If it's 16 000 000 MHz then how many seconds to overflow? it's from [0..65536] (16bits)
// Takes (65_536.0 / 16_000_000.0) = 0.004096 = 0.004seconds = 4miliseconds, I want 500 miliseconds, so 125 cycles for half a second

#include "pulga/core.h"
#include "pulga/pins.h"
#include "pulga/timer.h"

int main(void) {
    setPinMode(PIN_B1, OUTPUT);       // OC1A = PB1
    timer1_set_mode(TIMER1_MODE_CTC); // Set to CTC (TCCR1B => WGM12) see func. for refs.
    timer1_set_prescaler(CLK_1024);   // Set prescaler to 1024 (TCCR1B => CS12 & CS10)
    timer1_enable_oc1a_toggle();      // Set toggle for OC1A (TCCR1A => COM1A0)
//    timer1_set_toggle_period_ms(1000);
    timer1_set_toggle_frequency(1);   // 1 Hz (OCR1A => top) see func. for refs.

    while (1);
}


/*
int main(void) {
  // PB1 (OC1A) as output
  DDRB |= (1 << PB1);

  // Timer1 configuration
  TCCR1A = (1 << COM1A0);        // Toggle OC1A on compare match (this is a bit of the TCCR1A timer)
  TCCR1B = (1 << WGM12) |        // CTC mode, TOP = OCR1Ax (SET TO CTC)
            (1 << CS12) | (1 << CS10); // Prescaler 1024

  OCR1A = 7812; // Compare value for 1Hz toggle

  while (1) {
    // Empty loop — hardware timer toggles PB1 automatically
  }
}
*/
