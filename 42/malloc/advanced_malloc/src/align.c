#include "align.h"
#include <stdint.h> /* SIZE_MAX */

inline size_t align_up_sz(size_t x, size_t a) {
  size_t rem = x % a;
  if (rem == 0) return x;
  size_t add = a - rem;
  if (x > SIZE_MAX - add) return 0; /* overflow */
  return x + add;
}
