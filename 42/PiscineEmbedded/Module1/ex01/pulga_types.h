#ifndef PULGA_UTILS_H
#define PULGA_UTILS_H

typedef enum {
    PIN_A0, PIN_A1, PIN_A2, PIN_A3, PIN_A4, PIN_A5,
    PIN_B0, PIN_B1, PIN_B2, PIN_B3, PIN_B4, PIN_B5, PIN_B6, PIN_B7,
    PIN_C0, PIN_C1, PIN_C2, PIN_C3, PIN_C4, PIN_C5,
    PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_D4, PIN_D5, PIN_D6, PIN_D7
} Pin;

typedef enum { INPUT, OUTPUT } PinMode;
typedef enum { LOW=0, HIGH=1 } PinState;

#endif // PULGA_UTILS_H
