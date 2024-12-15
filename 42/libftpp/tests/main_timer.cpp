#include <iostream>
#include "timer.hpp"

int main() {
  Timer timer(2000);
  std::cout << "Timer started. Waiting..." << std::endl;

  while (!timer.hasTimedOut()) {
    ;
  }

  std::cout << "Timer expired!" << std::endl;
  return 0;
}
