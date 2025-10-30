#ifndef UART_H
#define UART_H

#include "core.h"

// ───────────────
// BAUD Rate
// ───────────────
#ifndef BAUD_RATE
# define BAUD_RATE 115200UL
#endif

#ifndef UART_BAUDRATE
# define UART_BAUDRATE BAUD_RATE
#endif

#define BAUD_PRESCALLER (((F_CPU / (UART_BAUDRATE * 8UL))) - 1) // Page 182 - Async. Double Speed Mode Formula

// Page 177, section 19
static inline void uart_init() {
  UCSR0A = (1 << U2X0); // Should be set according to Formula - Page 182
  UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8); // Set Baud Rate - Page 185 Code Example
  UBRR0L = (uint8_t)(BAUD_PRESCALLER); // Set Baud Rate - Page 185 Code Example
  UCSR0B = (1<<RXEN0)|(1<<TXEN0); // Enable Receiver & Transmiter - Page 185 Code Example
  // This is where we configure the data bits lenght,
  // parity check and number of stop bits, // Stop Bit
  UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01)); // 8N1 (8 bits 1 init-stop bit) Frame Format: using-the-usartserial-tutorial-part-2 (hekilledmywire.w>
}

static inline void uart_putc(unsigned char c) {
  if (c == '\n') uart_putc('\r');
  while(!(UCSR0A & (1<<UDRE0)));
  UDR0 = c;
}

static inline unsigned char uart_getc(void) {
 while(!(UCSR0A & (1<<RXC0)));
 return UDR0;
}

void uart_puts(const char* str) {
  while(*str != 0x00){    //Here we check if there is still more chars to send, this is done checking the actual char and see if it i>
    uart_putc(*str);
    str++;
  }
}

static inline void uart_flush(void) {
  while (UCSR0A & (1 << RXC0)) {
    (void)UDR0; // flush
  }
}


#endif
