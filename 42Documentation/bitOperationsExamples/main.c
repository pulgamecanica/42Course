#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int decimalToBinary(int num) {
  if (num == 0)
    return 0;
  else
    return (num % 2 + 10 * decimalToBinary(num / 2));
}

int main(int ac, char *av[]) {
  if (ac != 2) {
    printf("Error, wrong number of arguments\n");
    return (1);
  }
  uint32_t res = 1 << atoi(av[1]);
  printf(""
   "1=%08d | "
/* "%s=%08d | " */
   "%d=%08d\n",
   0b1,
/* av[1], decimalToBinary(atoi(av[1])), */
   res, decimalToBinary(res));
  printf("Result with 1<<%s = [%d]\n", av[1], res);
  return (0);
}
