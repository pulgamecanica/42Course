#include <iostream>
#include <cassert>
#include <cstring>

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

static void test_single_strcpy(char buff[], char buff_real[], const std::string& src) {
  static unsigned counter = 0;
  const std::string res = ft_strcpy(buff, src.c_str());
  const std::string res_real = strcpy(buff_real, src.c_str());
  printf("%u. %s%15s%s\t[%s|vs|%s]\n", ++counter, green.c_str(), src.c_str(), reset.c_str(), res.c_str(), res_real.c_str());
  assert(strcmp(res.c_str(), res_real.c_str()) == 0);
  assert(strcmp(buff, buff_real) == 0);
}

static void run_test_strcpy() {
  const std::string str1 = "pulgamecanica";
  const std::string str2 = "Don't Panic!";
  const std::string str3 = "0";
  const std::string str4 = "Turambar";
  const std::string str5 = "";

  char buff[100] = { 0 };
  char buff_real[100] = { 0 };
  test_single_strcpy(buff, buff_real, str1);
  test_single_strcpy(buff, buff_real, str2);
  test_single_strcpy(buff, buff_real, str3);
  test_single_strcpy(buff, buff_real, str4);
  test_single_strcpy(buff, buff_real, str5);
  test_single_strcpy(buff + 2, buff_real + 2, str1);
  test_single_strcpy(buff + 10, buff_real + 10, str2);
  ok("[ft_strcpy] Well done, all test passed!");
}


int main() {
  run_test_strlen();
  run_test_strcpy();
  return 0;
}