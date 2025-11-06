// main.c
// Module4 - exXX
// Behavior:
//  - SW1 (PD2 / INT0) increments a 4-bit value (0..15)
//  - SW2 (PD4 / PCINT20) decrements the value (0..15)
//  - value is shown permanently on LEDs D1..D4 (PB0, PB1, PB2, PB4) in binary
//  - everything driven by interrupts, main contains no register access

#include "pulga/pins.h"
#include "pulga/interrupt.h"
#include "pulga/timer.h"
#include <stdint.h>

// Hardware mapping
#define LED_D1  PIN_B0
#define LED_D2  PIN_B1
#define LED_D3  PIN_B2
#define LED_D4  PIN_B4

#define SW1     PIN_D2  // INT0
#define SW2     PIN_D4  // PCINT20

// Debounce settings (number of timer ticks). Timer runs every 20 ms.
#define DEBOUNCE_TICKS 3  // 3 * 20ms = 60 ms debounce

// Shared state (volatile because used in ISRs / callbacks)
volatile uint8_t g_value = 0;               // 0..15 displayed on LEDs
volatile uint8_t g_inc_debounce_active = 0; // set by INT0 callback
volatile uint8_t g_dec_debounce_active = 0; // set by PCINT callback

// Internal debounce counters (used inside timer callback)
static uint8_t s_inc_counter = 0;
static uint8_t s_dec_counter = 0;

// LED pins in bit order (LSB -> MSB)
static const Pin led_map[4] = { LED_D1, LED_D2, LED_D3, LED_D4 };

// Helper: update the 4 LEDs to reflect g_value (no direct registers)
static inline void update_leds(uint8_t value)
{
	for (int i = 0; i < 4; ++i) {
		if ((value >> i) & 1)
			writePin(led_map[i], LED_ON);
		else
			writePin(led_map[i], LED_OFF);
	}
}

// INT0 callback: button SW1 pressed (falling edge configured)
void on_sw1_press(void)
{
	// set debounce active flag, actual processing happens in timer
	g_inc_debounce_active = 1;
}

// PCINT2 callback: pin change on PD4 (SW2)
void on_sw2_change(void)
{
	// set debounce active flag; actual processing happens in timer
	g_dec_debounce_active = 1;
}

// Timer1 compare A callback, runs every 20 ms
void on_timer_tick(void)
{
	// Handle increment debounce
	if (g_inc_debounce_active) {
		s_inc_counter++;
		if (s_inc_counter >= DEBOUNCE_TICKS) {
			// reset counters/flags
			s_inc_counter = 0;
			g_inc_debounce_active = 0;

			// confirm button still pressed (buttons use pull-up => pressed == LOW)
			if (readPin(SW1) == BUTTON_PRESSED) {
				if (g_value < 15) {
					g_value++;
					update_leds(g_value);
				}
			}
		}
	} else {
		// ensure counter is 0 when not active
		s_inc_counter = 0;
	}

	// Handle decrement debounce
	if (g_dec_debounce_active) {
		s_dec_counter++;
		if (s_dec_counter >= DEBOUNCE_TICKS) {
			// reset counters/flags
			s_dec_counter = 0;
			g_dec_debounce_active = 0;

			// confirm button still pressed (buttons use pull-up => pressed == LOW)
			if (readPin(SW2) == BUTTON_PRESSED) {
				if (g_value > 0) {
					g_value--;
					update_leds(g_value);
				}
			}
		}
	} else {
		// ensure counter is 0 when not active
		s_dec_counter = 0;
	}
}

int main(void)
{
	// Configure LED pins as outputs and initialize OFF
	setPinMode(LED_D1, OUTPUT);
	setPinMode(LED_D2, OUTPUT);
	setPinMode(LED_D3, OUTPUT);
	setPinMode(LED_D4, OUTPUT);

	writePin(LED_D1, LED_OFF);
	writePin(LED_D2, LED_OFF);
	writePin(LED_D3, LED_OFF);
	writePin(LED_D4, LED_OFF);

	// Configure buttons as inputs with internal pull-ups
	setPinMode(SW1, INPUT);
	setPinMode(SW2, INPUT);
	writePin(SW1, HIGH); // enable pull-up
	writePin(SW2, HIGH); // enable pull-up

	// Attach callbacks for external interrupts (no direct regs)
	int0_set_mode(FALLING_EDGE);     // SW1 generates falling edge on press
	attach_int0(on_sw1_press);
	int0_enable();                   // enable INT0

	// Enable pin-change for PCINT2 (PD4) and attach callback
	int_pin_change_enable();         // enables PCIE2 and PCINT20 bit in mask
	interrupts_set_pcint2_callback(on_sw2_change);

	// Configure Timer1 CTC to tick every 20 ms, attach callback and enable
	timer1_init_ctc_mode(20);              // selects prescaler/OCR1A appropriately
	timer1_attach_compare_a(on_timer_tick);
	timer1_enable_compare_a_interrupt();

	// Initial LED update (shows 0)
	update_leds(g_value);

	// Enable global interrupts
	interrupts_enable();

	// Main loop does nothing; all work is interrupt-driven
	while (1) {
		/* nothing here - required by the exercise */
	}

	// never reached
	return 0;
}
