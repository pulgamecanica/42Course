#ifndef BUILTIN_RGB_H
#define BUILTIN_RBG_H

#include <util/delay.h>
#include "pins.h"
#include "timer.h"

#define LED_RED   PIN_D5
#define LED_GREEN PIN_D6
#define LED_BLUE  PIN_D3

const Pin RGB_PINS[3] = { LED_RED, LED_GREEN, LED_BLUE };

const int COLORS[7][3] = {
  /* RED */     {1, 0, 0},
  /* GREEN */   {0, 1, 0},
  /* BLUE */    {0, 0, 1},
  /* YELLOW */  {1, 1, 0},
  /* CYAN */    {0, 1, 1},
  /* MAGENTA */ {1, 0, 1},
  /* WHITE */   {1, 1, 1},
};

static void set_rgb(uint8_t r, uint8_t g, uint8_t b);
static inline void init_rgb() {
  for (int i = 0; i < 3; ++i)
    setPinMode(RGB_PINS[i], OUTPUT);
  timer0_init_fast_pwm(CLK_1024);
  timer2_init_fast_pwm(CLK_1024);
  set_rgb(0, 0, 0);
}

static inline void clear_rbg() {
  for (int i = 0; i < 3; ++i)
    writePin(RGB_PINS[i], LED_OFF);
}

static inline void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
//  "The double buffered Output Compare Register (OCR2A and OCR2B) are compared with the Timer/Counter
//    value at all times. The result of the compare can be used by the Waveform Generator to generate a PWM or
//    variable frequency output on the Output Compare pins (OC2A and OC2B)"
// Page 151 & 152
  OCR0B = r; // D5 → Timer0B → RED
  OCR0A = g; // D6 → Timer0A → GREEN
  OCR2B = b; // D3 → Timer2B → BLUE
}

static inline void wheel(uint8_t pos) {
  pos = 255 - pos;
  if (pos < 85) {
    set_rgb(255 - pos * 3, 0, pos * 3);
  } else if (pos < 170) {
    pos = pos - 85;
    set_rgb(0, pos * 3, 255 - pos * 3);
  } else {
    pos = pos - 170;
    set_rgb(pos * 3, 255 - pos * 3, 0);
  }
}

#endif
