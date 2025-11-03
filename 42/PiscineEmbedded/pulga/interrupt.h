#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "core.h"
#include <avr/interrupt.h>

// https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
static inline void interrupts_enable(void) { sei(); }
static inline void interrupts_disable(void) { cli(); }

// Page 113 - Timer0 overflow interrupt
static inline void timer0_enable_overflow_interrupt(void) {
  _SET(TIMSK0, TOIE0);
}
static inline void timer0_disable_overflow_interrupt(void) {
  _CLEAR(TIMSK0, TOIE0);
}

// Page 144 - Timer1 compare A interrupt
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

// Page 80 - External Interrupt Control Register A
// ISC01 ISC00: INT0 trigger mode
// ISC11 ISC10: INT1 trigger mode
// 00 = Low level, 01 = Any edge, 10 = Falling edge, 11 = Rising edge
#define INT0_LOW_LEVEL      0
#define INT0_ANY_EDGE       1
#define INT0_FALLING_EDGE   2
#define INT0_RISING_EDGE    3

#define INT1_LOW_LEVEL      0
#define INT1_ANY_EDGE       1
#define INT1_FALLING_EDGE   2
#define INT1_RISING_EDGE    3

static inline void int0_set_mode(uint8_t mode) {
	EICRA &= ~(_BIT(ISC01) | _BIT(ISC00));  // Clear ISC01 and ISC00
  EICRA |= (mode & 0x03);                 // Set trigger mode
}

static inline void int1_set_mode(uint8_t mode) {
	EICRA &= ~(_BIT(ISC11) | _BIT(ISC10));  // Clear ISC11 and ISC10
  EICRA |= ((mode & 0x03) << 2);          // Set trigger mode (shifted by 2)
}

// Page 81 - External Interrupt Mask Register
static inline void int0_enable(void) {
  _SET(EIMSK, INT0);
}

static inline void int0_disable(void) {
  _CLEAR(EIMSK, INT0);
}

static inline void int1_enable(void) {
  _SET(EIMSK, INT1);
}

static inline void int1_disable(void) {
  _CLEAR(EIMSK, INT1);
}

// Prepare a global static callbacks to be used for the compare A interrupt
// - Timer0 Overflow Interrupt
// - Timer1 Compare A Interrupt
// - RX Complete interrupt
// - External Interrupt 0
// - External Interrupt 1
static void (*timer0_ovf_callback)(void) = 0;
static void (*timer1_compa_callback)(void) = 0;
static void (*rx_complete_callback)(void) = 0;
static void (*int0_callback)(void) = 0;
static void (*int1_callback)(void) = 0;

// Setter of the global callbacks (call with NULL to reset)
void timer0_attach_overflow(void (*callback)(void)) {
  timer0_ovf_callback = callback;
}

void timer1_attach_compare_a(void (*callback)(void)) {
  timer1_compa_callback = callback;
}

void attach_rx_complete(void (*callback)(void)) {
  rx_complete_callback = callback;
}

void attach_int0(void (*callback)(void)) {
  int0_callback = callback;
}

void attach_int1(void (*callback)(void)) {
  int1_callback = callback;
}

// https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
ISR(TIMER0_OVF_vect) {
  if (timer0_ovf_callback)
    timer0_ovf_callback();
}

ISR(TIMER1_COMPA_vect) {
  if (timer1_compa_callback)
    timer1_compa_callback();
}

ISR(USART_RX_vect) {
  if (rx_complete_callback)
    rx_complete_callback();
}

// External Interrupt 0
ISR(INT0_vect) {
  if (int0_callback)
    int0_callback();
}

// External Interrupt 1
ISR(INT1_vect) {
  if (int1_callback)
    int1_callback();
}


#endif // TIMER_H

