#include "pulga/pins.h"

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

#define MICRO_SECONDS 700000

#define LED PIN_B0

int main(void) {
  setPinMode(LED, OUTPUT);
  while(42) {
    togglePin(LED);
    for (long int i=0;i<MICRO_SECONDS;++i);
  }
  return 0;
}
