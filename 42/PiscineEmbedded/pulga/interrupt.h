#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "core.h"
#include <avr/interrupt.h>

// https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
static inline void interrupts_enable(void) { sei(); }
static inline void interrupts_disable(void) { cli(); }

// Page 144
static inline void timer1_enable_compare_a_interrupt(void) {
  _SET(TIMSK1, OCIE1A);
}
static inline void timer1_disable_compare_a_interrupt(void) {
  _CLEAR(TIMSK1, OCIE1A);
}

// Page 201
static inline void enable_rx_complete_interrupt(void) {
  _SET(UCSR0B, RXCIE0);
}
static inline void disable_rx_complete_interrupt(void) {
  _CLEAR(UCSR0B, RXCIE0);
}

// Prepare a global static callbacks to be used for the compare A interrupt
// - Compare A Interrupt
// - RX Complete interrupt
static void (*timer1_compa_callback)(void) = 0;
static void (*rx_complete_callback)(void) = 0;

// Setter of the global callbacks (call with NULL to reset)
void timer1_attach_compare_a(void (*callback)(void)) {
  timer1_compa_callback = callback;
}

void attach_rx_complete(void (*callback)(void)) {
  rx_complete_callback = callback;
}

// https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
ISR(TIMER1_COMPA_vect) {
  if (timer1_compa_callback)
    timer1_compa_callback();
}

ISR(USART_RX_vect) {
  if (rx_complete_callback)
    rx_complete_callback();
}


#endif // TIMER_H

