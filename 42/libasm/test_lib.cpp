#include <iostream>
#include <cassert>

extern "C" size_t strlen(const char* str);

void run_test_strlen() {
  assert(strlen("42") == 2);
  assert(strlen("") == 0);
  assert(strlen("Hello Mars!") == 11);
  assert(strlen("1234567890") == 10);
  assert(strlen("Don't Panic!") == 12);
  assert(strlen("0000") == 4);
  std::cout << "Well done, all test passed!" << std::endl;
}

int main() {
  run_test_strlen();
  return 0;
}