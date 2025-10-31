#include "pulga/pins.h"
#include <util/delay.h>

#define LED_R PIN_D5
#define LED_G PIN_D6
#define LED_B PIN_D3

const Pin LED_PINS[3] = { LED_R, LED_G, LED_B };

int main(void) {
  int num = 0;
  for (int i = 0; i < 3; ++i)
    setPinMode(LED_PINS[i], OUTPUT);

  while (42) {
    for (int i = 0; i < 3; ++i)
      writePin(LED_PINS[i], LED_OFF);

    writePin(LED_PINS[num], LED_ON);
    _delay_ms(1000);
    num += 1;
    num %= 3;
  }
  return 0;
}
