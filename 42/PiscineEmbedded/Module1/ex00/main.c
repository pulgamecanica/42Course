#include "pulga_utils.h"

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

#define MICRO_SECONDS 500000

int main(void) {
  TO_OUTPUT(B, 0);
  while(42) {
    LED_TOGGLE(B,0);
    for (long int i=0;i<MICRO_SECONDS;++i);
  }
  return 0;
}
