#ifndef FT_ALLOC_HDR_H
#define FT_ALLOC_HDR_H

#include "align.h"    /* FT_ALIGN_UP, FT_MALLOC_ALIGN */
#include <stddef.h>    /* size_t */
#include <stdint.h>    /* uint8_t, uint16_t, uint32_t */

/* (compact, 1 byte) */
typedef uint8_t alloc_kind_t;
enum {
    FT_ALLOC_TINY  = 1,
    FT_ALLOC_SMALL = 2,
    FT_ALLOC_LARGE = 3
};

typedef struct ft_alloc_hdr {
    size_t       user_size;   /* useful for realloc/debug, this is the user requested size */
    union {
        struct { void   *zone; }  z;           /* TINY/SMALL (later) */
        struct { size_t  mapped_len; } lg;     /* LARGE path */
    } u;
    uint32_t     magic;       /* sanity check */
    alloc_kind_t kind;        /* TINY/SMALL/LARGE */
    uint8_t      flags;       /* reserved */
} ft_alloc_hdr_t;

/* Padded header size so (base + FT_ALLOC_HDR_ALIGNED_SIZE) is aligned */
enum { FT_ALLOC_HDR_ALIGNED_SIZE = FT_ALIGN_UP(sizeof(ft_alloc_hdr_t), FT_MALLOC_ALIGN) };

#if __STDC_VERSION__ >= 201112L
_Static_assert((FT_ALLOC_HDR_ALIGNED_SIZE % FT_MALLOC_ALIGN) == 0, "header not padded to A");
#endif

#endif /* FT_ALLOC_HDR_H */
