// main.c
// Module5 - ex00
// Read potentiometer (ADC0) every 20 ms and print its 8-bit value in hex.

#include "pulga/uart.h"
#include "pulga/adc.h"
#include "pulga/timer.h"
#include "pulga/interrupt.h"
#include <stdint.h>

// ─────────────────────────────────────────────────────────────
// Hex helper
// ─────────────────────────────────────────────────────────────
static void uart_puthex(uint8_t n)
{
	const char hex_digits[] = "0123456789ABCDEF";
	uart_putc(hex_digits[(n >> 4) & 0x0F]);
	uart_putc(hex_digits[n & 0x0F]);
}

// ─────────────────────────────────────────────────────────────
// 20 ms periodic task: read ADC and print
// ─────────────────────────────────────────────────────────────
void on_timer_tick(void)
{
	adc_start();
	while (!adc_is_complete());  // wait blocking
	uint8_t value = adc_read();  // 8-bit result
	adc_clear_flag();

	uart_puthex(value);
	uart_puts("\r\n");
}

// ─────────────────────────────────────────────────────────────
// main()
// ─────────────────────────────────────────────────────────────
int main(void)
{
	uart_init();        // setup UART 115200 8N1
	adc_init_default(); // setup ADC (8-bit, AVCC, channel 0)
  timer1_init_ctc_mode(20); // 20 ms periodic interrupt
	timer1_attach_compare_a(on_timer_tick);
	timer1_enable_compare_a_interrupt();
	interrupts_enable();

	uart_puts("ADC 8-bit potentiometer monitor\r\n");

	while (1) {
		// main loop idle, all periodic via timer
	}

	return 0;
}
