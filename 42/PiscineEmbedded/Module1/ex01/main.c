#include "pulga_utils.h"

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

#define MICRO_SECONDS 500000

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
*/

/*
 * TODO: Add SET/GET prescaler function to my API
*/

// If it's 16 000 000 MHz then how many seconds to overflow? it's from [0..65536] (16bits)
// Takes (65_536.0 / 16_000_000.0) = 0.004096 = 0.004seconds = 4miliseconds, I want 500 miliseconds, so 125 cycles for half a second

#include "pulga_utils.h"
#include "pulga_timer.h"

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

int main(void)
{
  TO_OUTPUT(B, 1);
  TIMER_16_SET_MODE(TIMER_16_CTC);
  TIMER_16_SET_COMPARE_TOGGLE('A');
  TIMER_16_SET_OCR('A', 7812);        // (F_CPU / (2 * PRESCALER * F_TOGGLE)) - 1
  TIMER_16_SET_PRESCALER(1024);

  while (1);

  return 0;
}

/*
int main(void)
{
    // PB1 = OC1A output
    DDRB |= (1 << DDB1); // Make the LED output

    // CTC mode (Clear Timer on Compare Match) - page 141 from RTFM
    // WGM12 = 1
    TCCR1B |= (1 << WGM12);

    // Toggle OC1A on compare match - same, page 141 from RTFM (Why A? Because we want Timer 16 bits A)
    // COM1A0 = 1 - `OC1A/PCINT1 – Port B, Bit 1` - page 92 from RTFM
    TCCR1A |= (1 << COM1A0);

    // Compare value for 0.5 s toggle (=> 1Hz blink)
    // OCR1A = (F_CPU / (2 * PRESCALER * F_TOGGLE)) - 1
    // For F_CPU = 16_000_000 Hz, PRESCALER = 1024, F_TOGGLE = 1 Hz / 2 = 0.5 Hz:
    // OCR1A = (16_000_000 / (2 * 1024 * 1)) - 1 = 7812
    OCR1A = 7812; // 16M / 1

    // Start timer with prescaler 1024
    // CS12 = 1, CS10 = 1  -> clk/1024
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // Infinite loop empty
    while (1)
        ;

    return 0;
}
*/



























