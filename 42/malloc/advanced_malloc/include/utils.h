#ifndef FT_UTILS_H
#define FT_UTILS_H

#include <stddef.h>   /* size_t */
#include <sys/mman.h> /* mmap & MACROS related */
#include <errno.h>    /* errno */

#define FT_MAX(a,b) ((a) > (b) ? (a) : (b))


/* MAP_ANONYMOUS shim for platforms that use MAP_ANON (e.g., macOS) */
#ifndef MAP_ANONYMOUS
#  define MAP_ANONYMOUS MAP_ANON
#endif
#define MMAP(sz) mmap(NULL, (sz), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)

#define FAIL_ENOMEM() do { errno = ENOMEM; return NULL; } while (0)

size_t pagesize_cached(void);

#endif /* FT_UTILS_H */
