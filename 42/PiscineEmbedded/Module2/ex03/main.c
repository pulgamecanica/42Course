#include "pulga/uart.h"
#include "pulga/utils.h"
#include "pulga/interrupt.h"

void uart_tx(char c) {
  uart_putc(c); // UDR0
}

// ISR for UART receive
void on_complete(void) {
  char c = UDR0;            // read received character
  uart_tx(ft_toggle_case(c));  // echo transformed
}

int main(void) {
  uart_init();              // initialize UART 115200 8N1
  attach_rx_complete(on_complete);
  enable_rx_complete_interrupt();
  interrupts_enable();
  while (42) ;
}

