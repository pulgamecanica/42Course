#ifndef ALLOC_HDR_H
# define ALLOC_HDR_H

# include <stddef.h>
# include <stdint.h>

/*
** ft_alloc_hdr_t - Allocation Header
**
** This header is placed immediately before each user allocation.
** When malloc returns a pointer, it actually returns (hdr + 1).
**
** Memory layout:
** [ft_alloc_hdr_t][user data...]
**                  ^ returned pointer
**
** The header allows us to:
** - Find the block metadata from a user pointer (for free/realloc)
** - Validate pointers passed to free()
** - Store size information for realloc
** - Add magic numbers for corruption detection
**
** Design decision: We store redundant information (size, zone pointer) to
** enable comprehensive debugging and validation at the cost of memory overhead.
** This is acceptable for a pedagogical implementation.
*/

typedef struct s_alloc_hdr
{
	uint32_t	magic;		/* Magic number for validation (0xDEADBEEF) */
	size_t		size;			/* Size requested by user (for realloc) */
	void		*block;			/* Pointer back to ft_block_t */
	void		*zone;			/* Pointer back to ft_zone_t */
}	t_alloc_hdr;

typedef t_alloc_hdr	ft_alloc_hdr_t;

/*
** FT_ALLOC_MAGIC - Magic number for allocation validation
**
** This constant is written to every allocation header.
** When free() or realloc() is called, we check this magic number
** to detect:
** - Invalid pointers (not from our malloc)
** - Corrupted memory (buffer overruns)
** - Double frees (we can zero the magic on free)
*/

# define FT_ALLOC_MAGIC 0xDEADBEEF

/*
** FT_ALLOC_HDR_SIZE - Size of allocation header (aligned)
**
** Note about Alignment and Padding Direction
** The padding goes to the RIGHT, for example:
**
**  Memory address:  0x1000         0x1010         0x1020
**                   |              |              |
**  Block Header:    [64 bytes of actual data....][padding to align next thing]
**                   ^                             ^
**                   Aligned start                 Next structure starts aligned
**
** This is a define (not sizeof) because we want the aligned size.
** The header must be aligned so that user data following it is also aligned.
*/

# define FT_ALLOC_HDR_SIZE (sizeof(ft_alloc_hdr_t))

/*
** ft_alloc_hdr_from_ptr()
**
** Retrieves the allocation header from a user pointer.
** Used by free() and realloc() to access allocation metadata.
**
** @param ptr: User pointer (as returned by malloc)
** @return: Pointer to allocation header
**
** Context: This is the inverse of the allocation process.
** malloc returns (header + 1), so we subtract to get the header.
*/

static inline ft_alloc_hdr_t	*ft_alloc_hdr_from_ptr(void *ptr)
{
	return ((ft_alloc_hdr_t *)ptr - 1);
}

/*
** ft_alloc_hdr_to_ptr()
**
** Gets the user pointer from an allocation header.
** Used after creating an allocation to return the proper address to the user.
**
** @param hdr: Pointer to allocation header
** @return: User pointer
*/

static inline void	*ft_alloc_hdr_to_ptr(ft_alloc_hdr_t *hdr)
{
	return ((void *)(hdr + 1)); // same as (void *)((uint8_t *)hdr + sizeof(ft_alloc_hdr_t))
}

/*
** ft_alloc_hdr_is_valid()
**
** Validates an allocation header by checking the magic number.
** Used by free() and realloc() to detect invalid pointers.
**
** @param hdr: Pointer to allocation header
** @return: 1 if valid, 0 otherwise
**
** Context: Protects against common malloc errors:
** - free(stack_variable)
** - free(0x12345678) [random pointer]
** - double free (if we zero magic on free)
*/

static inline int	ft_alloc_hdr_is_valid(const ft_alloc_hdr_t *hdr)
{
	return (hdr && hdr->magic == FT_ALLOC_MAGIC);
}

#endif

