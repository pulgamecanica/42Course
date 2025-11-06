#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "core.h"
#include <avr/interrupt.h>

// Page 80 - External Interrupt Control Register A
// ISC01 ISC00: INT0 trigger mode
// ISC11 ISC10: INT1 trigger mode
// 00 = Low level, 01 = Any edge, 10 = Falling edge, 11 = Rising edge
typedef enum {
  LOW_LEVEL,
  ANY_EDGE,
  FALLING_EDGE,
  RISING_EDGE
} IntTriggerMode;

// https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
static inline void interrupts_enable(void) { sei(); }
static inline void interrupts_disable(void) { cli(); }

// Page 113 - Timer0 overflow interrupt
static inline void timer0_enable_overflow_interrupt(void) {
  _SET(TIMSK0, TOIE0);
  TCNT0 = 0;
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

// Page 80
static inline void int0_set_mode(IntTriggerMode mode) {
  EICRA &= ~(_BIT(ISC01) | _BIT(ISC00));  // Clear ISC01 and ISC00
  EICRA |= (mode & 0x03);                 // Set trigger mode
}

static inline void int1_set_mode(IntTriggerMode mode) {
  EICRA &= ~(_BIT(ISC11) | _BIT(ISC10));  // Clear ISC11 and ISC10
  EICRA |= ((mode & 0x03) << 2);          // Set trigger mode (shifted by 2)
}

// Page 81 - External Interrupt Mask Register
// Page 97: Why we use INT0 and INT1? RTFM
static inline void int0_enable(void) {
  _SET(EIMSK, INT0); // INT on PD2
}

static inline void int0_disable(void) {
  _CLEAR(EIMSK, INT0);
}

static inline void int1_enable(void) {
  _SET(EIMSK, INT1); // INT on PD3
}

static inline void int1_disable(void) {
  _CLEAR(EIMSK, INT1);
}

// Enable Pin Change Interrupt 2 (PCINT20)
// Page 82 & Page 97
static inline void int_pin_change_enable(void) {
  _SET(PCICR, PCIE2);
  _SET(PCMSK2, PCINT20); // Pin PD4 (Page 97)
}

static inline void int_pin_change_edisable(void) {
  _CLEAR(PCICR, PCIE2);
}

// AVOID Debounce on Interrupt
// https://www.avrfreaks.net/s/topic/a5C3l000000UWEpEAO/t137717?comment=P-1483338


// Prepare a global static callbacks to be used for the compare A interrupt
// - Timer0 Overflow Interrupt
// - Timer1 Compare A Interrupt
// - RX Complete interrupt
// - External Interrupt 0
// - External Interrupt 1
typedef void (*interrupt_callback_t)(void);

static interrupt_callback_t timer0_ovf_callback = 0;
static interrupt_callback_t timer1_compa_callback = 0;
static interrupt_callback_t rx_complete_callback = 0;
static interrupt_callback_t int0_callback = 0;
static interrupt_callback_t int1_callback = 0;
static interrupt_callback_t pcint2_callback = 0;

// Setter of the global callbacks (call with NULL to reset)
void timer0_attach_overflow(interrupt_callback_t cb) {
  timer0_ovf_callback = cb;
}

void timer1_attach_compare_a(interrupt_callback_t cb) {
  timer1_compa_callback = cb;
}

void attach_rx_complete(interrupt_callback_t cb) {
  rx_complete_callback = cb;
}

void attach_int0(interrupt_callback_t cb) {
  int0_callback = cb;
}

void attach_int1(interrupt_callback_t cb) {
  int1_callback = cb;
}

void interrupts_set_pcint2_callback(interrupt_callback_t cb) {
  pcint2_callback = cb;
}

// MY own ISR implementation MOAHAHAHAHAAH
#define PULGA_ISR(vector) \
    void vector(void) __attribute__((signal, used, externally_visible)); \
    void vector(void)

// Vector aliases for ATmega328P
#define INT0_vect        __vector_1
#define INT1_vect        __vector_2
#define PCINT2_vect      __vector_5
#define TIMER1_COMPA_vect __vector_11
#define TIMER0_OVF_vect  __vector_16
#define USART_RX_vect    __vector_18

// https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
PULGA_ISR(TIMER0_OVF_vect) {
  if (timer0_ovf_callback)
    timer0_ovf_callback();
}

PULGA_ISR(TIMER1_COMPA_vect) {
  if (timer1_compa_callback)
    timer1_compa_callback();
}

PULGA_ISR(USART_RX_vect) {
  if (rx_complete_callback)
    rx_complete_callback();
}

// External Interrupt 0
PULGA_ISR(INT0_vect) {
  if (int0_callback)
    int0_callback();
  EIFR = (1 << INTF0);
}

// External Interrupt 1
PULGA_ISR(INT1_vect) {
  if (int1_callback)
    int1_callback();
}

PULGA_ISR(PCINT2_vect) {
  if (pcint2_callback)
    pcint2_callback();
  PCIFR = (1 << PCIF2);
}

void interrupts_init(void) {
  timer0_enable_overflow_interrupt();
  timer1_enable_compare_a_interrupt();
  enable_rx_complete_interrupt();
  int0_enable();
  int1_enable();
  int0_set_mode(FALLING_EDGE);
  int1_set_mode(FALLING_EDGE);
  int_pin_change_enable();
}


#endif // INTERRUPT_H

