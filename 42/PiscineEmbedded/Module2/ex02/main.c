#include <avr/interrupt.h>
#include "pulga/uart.h"
#include "pulga/timer.h"

void uart_tx(char c) {
  uart_putc(c); // UDR0
}

char uart_rx(void) {
  return uart_getc();
}

int main(void) {
  uart_init();            // Configure UART: 115200 8N1
  while (1)
  {
      char c = uart_rx(); // Wait for a character
      uart_tx(c);         // Echo it back
  }
  return 0;
}
