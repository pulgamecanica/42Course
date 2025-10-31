#ifndef UART_H
#define UART_H

#include "core.h"
#include "utils.h"

#define BACKSPACE  0x7F
#define ENTER      '\r'
#define NEWLINE    '\n'

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

static inline void uart_putnum(uint8_t n) {
  char buf[4];
  int i = 0;
  if (n == 0)
    uart_putc('0');
  else {
    while (n > 0) {
      buf[i++] = (n % 10) + '0';
      n /= 10;
    }
    while (i--)
      uart_putc(buf[i]);
  }
}

static inline void uart_flush(void) {
  while (UCSR0A & (1 << RXC0)) {
    (void)UDR0; // flush
  }
}

// Reads user input until ENTER (blocking).
// Echoes with callback function.
// Handles Backspace.
// Writes the result (null-terminated) into buffer.
// Returns the number of characters read (not counting '\0').
static inline uint8_t uart_get_input_callback(char *buffer, uint8_t max_len, void (*echo_callback)(unsigned char)) {
  uint8_t i = 0;
  char c;
  while (1) {
    c = uart_getc();
    if (c == ENTER || c == NEWLINE) {
      uart_puts("\r\n");
      break;
    }
    else if (c == BACKSPACE) {
      if (i > 0) {
        i--;
        uart_puts("\b \b"); // move back, erase, move back again
      }
    }
    else if (i < max_len - 1) {
      buffer[i++] = c;
      echo_callback(c); // echo
    }
  }
  buffer[i] = '\0';
  return i;
}

static inline uint8_t uart_get_input(char *buffer, uint8_t max_len) {
  return uart_get_input_callback(buffer, max_len, uart_putc);
}

static inline void uart_putsecret(unsigned char c) {
  (void)c;
  uart_putc('*');
}

static inline uint8_t uart_get_input_secret(char *buffer, uint8_t max_len) {
    return uart_get_input_callback(buffer, max_len, uart_putsecret);
}

#endif
