#include "pulga/pins.h"
#include <util/delay.h>

#define LED_R PIN_D5
#define LED_G PIN_D6
#define LED_B PIN_D3

const Pin LED_PINS[3] = { LED_R, LED_G, LED_B };

const int COLORS[7][3] = { 
  /* RED */     {1, 0, 0},
  /* GREEN */   {0, 1, 0},
  /* BLUE */    {0, 0, 1},
  /* YELLOW */  {1, 1, 0},
  /* CYAN */    {0, 1, 1},
  /* MAGENTA */ {1, 0, 1},
  /* WHITE */   {1, 1, 1},
};

void set_rgb_color(int color_i) {
  for (int i = 0; i < 3; ++i) 
    writePin(LED_PINS[i], COLORS[color_i][i] ? LED_ON : LED_OFF);
}

int main(void) {
  int num = 0;
  for (int i = 0; i < 3; ++i)
    setPinMode(LED_PINS[i], OUTPUT);

  while (42) {
    for (int i = 0; i < 3; ++i)
      writePin(LED_PINS[i], LED_OFF);
    set_rgb_color(num);
    _delay_ms(1000);
    num += 1;
    num %= 7;
  }
  return 0;
}
