#include <iostream>
#include <cassert>

extern "C" size_t ft_strlen(const char* str);

void run_test_strlen() {
  assert(ft_strlen("42") == 2);
  assert(ft_strlen("") == 0);
  assert(ft_strlen("Hello Mars!") == 11);
  assert(ft_strlen("1234567890") == 10);
  assert(ft_strlen("Don't Panic!") == 12);
  assert(ft_strlen("0000") == 4);
  std::cout << "Well done, all test passed!" << std::endl;
}

int main() {
  run_test_strlen();
  return 0;
}