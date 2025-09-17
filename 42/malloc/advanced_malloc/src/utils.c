#include "utils.h"
#include <unistd.h>   /* sysconf */

inline size_t pagesize_cached(void) {
  static size_t ps = 0;
  if (!ps) {
    long lps = sysconf(_SC_PAGESIZE);
    ps = (lps > 0) ? (size_t)lps : 4096u;
  }
  return ps;
}
