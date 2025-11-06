// main.c
// Module5 - ex00
// Read potentiometer (ADC0) every 20 ms and print its 8-bit value in hex.
#include <avr/io.h>
#include <stdint.h>

void __vector_1(void) __attribute__((signal, used, externally_visible));

void __vector_1(void) {
    PORTB ^= (1 << PB2);
}

#include "pulga/interrupt.h"


int main(void) {
    DDRB |= (1 << PB1);     // LED output
    DDRD &= ~(1 << PD2);    // Button input

    EICRA = (1 << ISC01);   // Falling edge on INT0
    EIMSK = (1 << INT0);    // Enable INT0
    sei();                  // Enable global interrupts

    while (1) {
        // Main loop runs freely; interrupt toggles LED
    }
}
