#include <iostream>
#include "observable_value.hpp"

int main() {
  ObservableValue<int> observable_int(10);

  observable_int.subscribe([](int new_value) {
    std::cout << "Subscriber 1: Value changed to " << new_value << "\n";
  });

  std::cout << "Initial value: " << observable_int.get() << "\n";
  observable_int.set(20);
  observable_int.subscribe([](int new_value) {
    std::cout << "Subscriber 2: New value received: " << new_value << "\n";
  });
  observable_int.set(30);

  return 0;
}
