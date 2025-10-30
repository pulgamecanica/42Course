#include "pulga/core.h"
#include "pulga/uart.h"
#include <util/delay.h>

void uart_tx(char c) {
  uart_putc(c); // UDR0
}

int main(void) {
  uart_init(); // USCR0A, UBRR0H, UBRR0L UCSR0B & UCSR0C (see uart.h for more refs.)
  while(1) {
    uart_putc('Z'); // Send char over the serial port
    _delay_ms(1000); // 1Hz freq
  }
  return 0;
}
