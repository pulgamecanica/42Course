#include <iostream>
#include "chronometer.hpp"
#include <thread>

int main() {
  Chronometer chrono;
  chrono.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  int64_t elapsed = chrono.stop();
  std::cout << "Elapsed time: " << elapsed << " ms" << std::endl;
  return 0;
}
