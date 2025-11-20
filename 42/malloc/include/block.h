#ifndef BLOCK_H
# define BLOCK_H

# include <stddef.h>
# include <stdint.h>

/*
** ft_block_t - Memory Block Metadata
**
** Each zone is divided into blocks. A block represents either:
** - A free region available for allocation
** - An allocated region in use by the program
**
** Blocks are tracked in two ways:
** 1. Address-order list (all blocks, for coalescing neighbors)
** 2. Free list (only free blocks, for allocation)
**
** Memory layout within a zone:
** [ft_block_t][padding][ft_alloc_hdr_t][user data...][ft_block_t][...]
**
** Design decision: We use double-linked lists for both the address-order
** list and free list. This allows O(1) removal and easy coalescing of
** adjacent free blocks.
*/

typedef struct s_block
{
	size_t			size;			/* Total size of this block (including metadata) */
	uint8_t			is_free;	/* 1 if free, 0 if allocated */

	/* Address-order list - tracks all blocks in memory order (also free blocks -> ALL) */
	struct s_block	*prev;			/* Previous block in address order */
	struct s_block	*next;			/* Next block in address order */

	/* Free list - only links free blocks for quick allocation */
	struct s_block	*prev_free;		/* Previous free block */
	struct s_block	*next_free;		/* Next free block */

	/* Back reference to containing zone */
	void			*zone;			/* Pointer to ft_zone_t */
}	t_block;

typedef t_block	ft_block_t;

/*
** FT_BLOCK_HDR_SIZE - Size of block header (aligned)
**
** This is a define to be consistent with our other size constants.
** The block header sits at the beginning of each memory block.
*/

# define FT_BLOCK_HDR_SIZE (sizeof(ft_block_t))

/*
** FT_MIN_BLOCK_SIZE - Minimum useful block size
**
** A block must be at least large enough to hold:
** - Block metadata (ft_block_t)
** - Allocation header (ft_alloc_hdr_t)
** - At least 1 byte of user data
** - Proper alignment
**
** This prevents excessive fragmentation from tiny allocations.
** Used when splitting blocks to ensure the remainder is useful.
*/

# define FT_MIN_BLOCK_SIZE (FT_BLOCK_HDR_SIZE + 32)

/*
** ft_block_data_ptr()
**
** Gets the pointer to usable data area in a block.
** This is where the allocation header (and then user data) goes.
**
** @param block: Pointer to block
** @return: Pointer to data area (just after block header)
**
** Context: When allocating from a block, we place the allocation header
** here, then return (alloc_header + 1) to the user.
*/

static inline void	*ft_block_data_ptr(ft_block_t *block)
{
	return ((void *)((uint8_t *)block + FT_BLOCK_HDR_SIZE));
}

/*
** ft_block_from_data_ptr()
**
** Retrieves the block header from a data pointer.
** This is the inverse of ft_block_data_ptr().
**
** @param ptr: Pointer to data area
** @return: Pointer to block header
**
** Context: Used when we have an allocation header and need to find
** the block it belongs to (though typically we store block pointer
** in the allocation header itself).
*/

static inline ft_block_t	*ft_block_from_data_ptr(void *ptr)
{
	return ((ft_block_t *)((uint8_t *)ptr - FT_BLOCK_HDR_SIZE));
}

/*
** Block manipulation functions (implemented in block.c)
*/

/*
** ft_block_init()
**
** Initializes a new block at the given address.
** Sets up size, marks as free, and clears all pointers.
**
** @param addr: Address where block should be initialized
** @param size: Total size of the block (including header)
** @param zone: Pointer to parent zone
** @return: Pointer to initialized block
*/

ft_block_t	*ft_block_init(void *addr, size_t size, void *zone);

/*
** ft_block_split()
**
** Splits a free block into two blocks if there's enough space.
** The first block will have the requested size, and the remainder
** becomes a new free block.
**
** @param block: Block to split
** @param size: Desired size for first block (including all headers)
** @return: Pointer to remainder block, or NULL if not enough space to split
**
** Context: Called during allocation when a free block is larger than needed.
** This reduces internal fragmentation by making the excess available for
** future allocations.
*/

ft_block_t	*ft_block_split(ft_block_t *block, size_t size);

/*
** ft_block_can_merge()
**
** Checks if two adjacent blocks can be merged.
** Blocks can merge if they are adjacent in memory and both are free.
**
** @param a: First block
** @param b: Second block
** @return: 1 if mergeable, 0 otherwise
**
** Context: Used during free() to coalesce adjacent free blocks,
** reducing external fragmentation.
*/

int			ft_block_can_merge(ft_block_t *a, ft_block_t *b);

/*
** ft_block_merge()
**
** Merges two adjacent free blocks into one larger block.
** The second block is absorbed into the first.
**
** @param a: First block (will be extended)
** @param b: Second block (will be absorbed)
**
** Context: Called after free() to coalesce adjacent free blocks.
** This is essential for preventing external fragmentation.
*/

void		ft_block_merge(ft_block_t *a, ft_block_t *b);

#endif

