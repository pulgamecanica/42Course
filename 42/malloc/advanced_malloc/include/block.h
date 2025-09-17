#ifndef FT_BLOCK_H
#define FT_BLOCK_H

#include "align.h"
#include <stddef.h>
#include <stdint.h>

/*
 * A free list serves as a reference on how the blocks are organized
 * which areas are available and which areas have been splited.
 * Also having this kind of structure allows coalescing when two or more
 * adjacent areas become free. Preventing fragmentation.
 * 
 * Although it is indeed a linked list, soon I realized that I cannot
 * allocate memory for the nodes, each node, uses it's own "content" pointer
 * and adds the "block" before the content. This means that the memory allocated
 * should at least be able to hold the block, because once we free the memory
 * then the area 'arena' will be 'recycled' and we need to add it to the free list.
 * For this we need at least to set up the block pointers.
 * 
 * Isn't this extreme? I thought poiterst have like 8byes, this means the size min is
 * 2 poitners + size_t + uint8_t? that's like 8bytes*2 + 4byes + 1 byte isn't it?
 */

/* Minimal free-list block used by ft_first_fit.
 *   - size: aligned payload bytes available in this block
 *   - prev_free/next_free: links in the free list (double linked list)
 *   - is_free: nonzero when on the free list (helps during debugging)
 */
typedef struct ft_block {
    size_t          size;
    struct ft_block *prev_free;
    struct ft_block *next_free;
    uint8_t         is_free;
} ft_block_t;

/**
 * the Block is always present in the memory context, but when allocated it's removed
 * from the free list, when freed, it can easily return to the zone 
 */

/* Padded header size so (base + FT_BLOCK_ALIGNED_SIZE) is aligned */
enum { FT_BLOCK_ALIGNED_SIZE = FT_ALIGN_UP(sizeof(ft_block_t), FT_MALLOC_ALIGN) };

#if __STDC_VERSION__ >= 201112L
_Static_assert((FT_BLOCK_ALIGNED_SIZE % FT_MALLOC_ALIGN) == 0, "header not padded to A");
#endif

#endif /* FT_BLOCK_H */
