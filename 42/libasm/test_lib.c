#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

extern size_t ft_strlen(const char* str);
extern char *ft_strcpy(char *restrict dst, const char *restrict src);
extern int ft_strcmp(const char *s1, const char *s2);

const char *pink = "\033[38;5;205m";  // Pink text
const char *bold = "\033[1m";         // Bold text
const char *reset = "\033[0m";        // Reset formatting to default
const char *green = "\033[32m";       // Green text

static void ok(const char* msg) {
  printf("✨%s%s%s%s🌟\n\n", bold, pink, msg, reset);
}

static void test_single_strlen(const char* str) {
  static unsigned counter = 0;
  const size_t res = ft_strlen(str);
  const size_t res_real = strlen(str);
  printf("%u. %s%15s%s\t[%3lu|vs|%3lu]\n", ++counter, green, str, reset, res, res_real);
  assert(res == res_real);
}

static void run_test_strlen() {
  test_single_strlen("42");
  test_single_strlen("");
  test_single_strlen("");
  test_single_strlen("Hello Mars!");
  test_single_strlen("1234567890");
  test_single_strlen("Don't Panic!");
  test_single_strlen("0000");
  ok("[ft_strlen] Well done, all test passed!");
}

static void test_single_strcpy(char buff[], char buff_real[], const char* src) {
  static unsigned counter = 0;
  const char* res = ft_strcpy(buff, src);
  const char* res_real = strcpy(buff_real, src);
  printf("%u. %s%15s%s\t[%s|vs|%s]\n", ++counter, green, src, reset, res, res_real);
  assert(strcmp(res, res_real) == 0);
  assert(strcmp(buff, buff_real) == 0);
}

static void run_test_strcpy() {
  const char  str1[] = "pulgamecanica";
  const char  str2[] = "Don't Panic!";
  const char  str3[] = "0";
  const char  str4[] = "Turambar";
  const char  str5[] = "";

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

static void test_single_strcmp(const char* str1, const char* str2) {
  static unsigned counter = 0;
  int res = ft_strcmp(str1, str2);
  int res_real = strcmp(str1, str2);
  printf("%u. %s%10s[-cmp-]%-10s%s\t[%3d|vs|%3d]\n", ++counter, green, str1, str2, reset, res, res_real);
  assert(res == res_real);
}

static void run_test_strcmp() {
  test_single_strcmp("pulga", "pulga");
  test_single_strcmp("PPU", "PPUU");
  test_single_strcmp("", "");
  test_single_strcmp("", "0");
  test_single_strcmp("0", "");
  test_single_strcmp("SomeText", "sometext");
  test_single_strcmp("soooO", "soooOo");
  test_single_strcmp("soooO", "sooo");
  test_single_strcmp("same", "same");
  test_single_strcmp("same42*42same", "same42*42same");
  ok("[ft_strcmp] Well done, all test passed!");
}

int main() {
  run_test_strlen();
  run_test_strcpy();
  run_test_strcmp();
  return 0;
}