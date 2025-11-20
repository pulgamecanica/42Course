#ifndef ALIGN_H
# define ALIGN_H

# include <stddef.h>
# include <stdint.h>

/*
** Alignment is critical for memory allocators for both performance and
** correctness. Modern architectures require certain data types to be aligned
** to specific boundaries (e.g., a pointer should be aligned to 8 bytes on
** 64-bit systems).
**
** FT_ALIGN_SIZE defines our allocation alignment boundary (16 bytes):
** - Satisfies alignment requirements for all primitive types
** - Optimizes CPU cache line usage
** - Required by malloc(3) specification for maximum alignment
*/

# define FT_ALIGN_SIZE 16

/*
** FT_ALIGN_UP(x, alignment)
**
** Rounds 'x' up to the nearest multiple of 'alignment'.
** Used to ensure all allocations and metadata are properly aligned.
**
** Example: FT_ALIGN_UP(13, 16) = 16
**          FT_ALIGN_UP(16, 16) = 16
**          FT_ALIGN_UP(17, 16) = 32
**
** Implementation note:
** (x + alignment - 1) & ~(alignment - 1)
** This works only when alignment is a power of 2, which is always true
** for our use case. (2, 4, 8, 16, 32, 64, ...)
*/

# define FT_ALIGN_UP(x, alignment) \
	(((x) + (alignment) - 1) & ~((alignment) - 1))

/*
** FT_IS_ALIGNED(ptr, alignment)
**
** Checks if a pointer is aligned to the specified boundary.
** Used for validation and debugging.
**
** Returns: 1 if aligned, 0 otherwise
*/

# define FT_IS_ALIGNED(ptr, alignment) \
	(((uintptr_t)(ptr) & ((alignment) - 1)) == 0)

/*
** ft_align_up_ptr()
**
** Aligns a pointer up to the specified boundary.
** Used when calculating positions in memory zones.
**
** @param ptr: Pointer to align
** @param alignment: Alignment boundary (must be power of 2)
** @return: Aligned pointer
*/

static inline void	*ft_align_up_ptr(void *ptr, size_t alignment)
{
	uintptr_t	addr;

	addr = (uintptr_t)ptr;
	return ((void *)FT_ALIGN_UP(addr, alignment));
}

#endif

