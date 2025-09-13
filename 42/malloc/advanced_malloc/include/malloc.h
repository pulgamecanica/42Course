#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <stddef.h>    /* size_t */
#include <stdint.h>    /* uint8_t, uint16_t, uint32_t */
#include <stdalign.h>  /* max_align_t, _Alignof */

/* ===== Public API ===== */
#ifdef __cplusplus
extern "C" {
#endif

void *malloc(size_t size);
void  free(void *ptr);
void  show_alloc_mem(void); /* stub is fine for now */

#ifdef __cplusplus
} /* extern "C" */
#endif

/* ===== Config / utils ===== */
#define FT_MIN_ALIGN 16u
#define FT_MAX(a,b) ((a) > (b) ? (a) : (b))
#define FT_ALIGN_UP(x,a) (((x) + (a) - 1) / (a) * (a))

#ifndef FT_TINY_MAX
#  define FT_TINY_MAX   512UL
#endif
#ifndef FT_SMALL_MAX
#  define FT_SMALL_MAX  8192UL
#endif

/* Classes (compact, 1 byte) */
typedef uint8_t alloc_kind_t;
enum {
    ALLOC_TINY  = 1,
    ALLOC_SMALL = 2,
    ALLOC_LARGE = 3
};

/* ---- unified per-allocation header (sits right before user ptr) ---- */
#define FT_MALLOC_MAGIC  0xC0FFEE01u

typedef struct alloc_hdr {
    size_t       user_size;   /* useful for realloc/debug */
    union {
        struct { void   *zone; }  z;           /* TINY/SMALL (later) */
        struct { size_t  mapped_len; } lg;     /* LARGE path */
    } u;
    uint32_t     magic;       /* sanity check */
    alloc_kind_t kind;        /* TINY/SMALL/LARGE */
    uint8_t      flags;       /* reserved */
    uint16_t     _pad;        /* keep natural 8B boundary */
} alloc_hdr_t;

/* Don’t depend on alloc_hdr_t being complete above.
   We care about user data alignment (>= max_align_t). */
enum { FT_MALLOC_ALIGN = FT_MAX(FT_MIN_ALIGN, _Alignof(max_align_t)) };

/* Padded header size so (base + FT_HDR_SIZE) is aligned */
enum { FT_HDR_ALIGNED_SIZE = FT_ALIGN_UP(sizeof(alloc_hdr_t), FT_MALLOC_ALIGN) };

#if __STDC_VERSION__ >= 201112L
_Static_assert((FT_HDR_ALIGNED_SIZE % FT_MALLOC_ALIGN) == 0, "header not padded to A");
#endif

#endif /* FT_MALLOC_H */
