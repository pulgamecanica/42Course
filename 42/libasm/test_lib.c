#include <stdio.h>
#include <unistd.h>
#include <assert.h>

extern size_t strlen(const char* str);

void run_test_strlen() {
  const char * msg = "Well done, all test passed!\n";

  assert(strlen("42") == 2);
  assert(strlen("") == 0);
  assert(strlen("Hello Mars!") == 11);
  assert(strlen("1234567890") == 10);
  assert(strlen("Don't Panic!") == 12);
  assert(strlen("0000") == 4);
  write(1, msg, strlen(msg));
}

int main() {
  run_test_strlen();
  return 0;
}