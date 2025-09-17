#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <stddef.h>    /* size_t */

/*
 * ===== Public API =====
 */
#ifdef __cplusplus
extern "C" {
#endif

void *malloc(size_t size);
void  free(void *ptr);
void  show_alloc_mem(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

/*
 * ===== ARENA SIZES =====
 */
#ifndef FT_TINY_MAX
#  define FT_TINY_MAX   512UL
#endif /* FT_TINY_MAX */

#ifndef FT_SMALL_MAX
#  define FT_SMALL_MAX  8192UL
#endif /* FT_SMALL_MAX */

/*
 * ---- unified per-allocation header (sits right before user ptr) ----
 */
#define FT_MALLOC_MAGIC  0xC0FFEE01u

#endif /* FT_MALLOC_H */
