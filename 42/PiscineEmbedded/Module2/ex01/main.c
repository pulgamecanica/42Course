#include "pulga/uart.h"
#include "pulga/timer.h"
#include "pulga/interrupt.h"

void uart_printstr(const char *str) {
  while (*str)
    uart_putc(*str++);
}

void on_timer1_tick(void) {
  uart_printstr("Hello World!\n");
}

int main(void) {
  uart_init();                              // Configure UART: 115200 8N1
  timer1_set_mode(TIMER1_MODE_CTC);         // Timer1 in CTC mode
  timer1_set_prescaler(CLK_1024);           // Prescaler = 1024
  timer1_set_ctc_interrupt_period_ms(2000);
  timer1_attach_compare_a(on_timer1_tick);  // Set the interrupt ctc callback
  timer1_enable_compare_a_interrupt();      // TIMSK1 => OCIE1A
  interrupts_enable();
  while (42) ;
}
