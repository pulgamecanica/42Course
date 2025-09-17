#ifndef FT_ALIGN_H
#define FT_ALIGN_H

#include "utils.h"    /* FT_MAX */
#include <stddef.h>   /* size_t, max_align_t */
#include <stdalign.h> /* _Alignof */

#define FT_MIN_ALIGN 16u
#define FT_ALIGN_UP(x,a) (((x) + (a) - 1) / (a) * (a))

// Align up with overflow protection, on error 0 is returned.
size_t align_up_sz(size_t x, size_t a);

/*
 * Don’t depend on alloc_hdr_t being complete above.
 * We care about user data alignment (>= max_align_t).
 * This is the base align, for MALLOC
 */
enum { FT_MALLOC_ALIGN = FT_MAX(FT_MIN_ALIGN, _Alignof(max_align_t)) };

#endif /* FT_ALIGN_H */
