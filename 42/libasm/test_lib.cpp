#include <iostream>
#include <cassert>

#include "libasm.h"

const std::string pink = "\033[38;5;205m";  // Pink text
const std::string bold = "\033[1m";         // Bold text
const std::string reset = "\033[0m";        // Reset formatting to default
const std::string green = "\033[32m";       // Green text

static void ok(const std::string& msg) { 
  std::cout <<"✨" << bold + pink + msg + reset << "🌟" << std::endl << std::endl;
}

void run_test_strlen() {
  assert(ft_strlen("42") == 2);
  assert(ft_strlen("") == 0);
  assert(ft_strlen("Hello Mars!") == 11);
  assert(ft_strlen("1234567890") == 10);
  assert(ft_strlen("Don't Panic!") == 12);
  assert(ft_strlen("0000") == 4);
  ok("[strlen] Well done, all test passed!");
}

int main() {
  run_test_strlen();
  return 0;
}