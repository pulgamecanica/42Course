#ifndef MALLOC_H
# define MALLOC_H

# include <stdlib.h>

/*
** Public API - Standard malloc interface
**
** These functions replace the libc malloc, free, and realloc.
** They must have identical signatures and behavior to the system versions.
*/

/*
** malloc()
**
** Allocates size bytes and returns a pointer to the allocated memory.
** The memory is not initialized. If size is 0, behavior is implementation-
** defined (we return NULL).
**
** @param size: Number of bytes to allocate
** @return: Pointer to allocated memory, or NULL on failure
**
** Thread safety: This function is thread-safe (uses mutex internally)
*/

void	*malloc(size_t size);

/*
** free()
**
** Frees the memory space pointed to by ptr, which must have been returned
** by a previous call to malloc() or realloc(). If ptr is NULL, no operation
** is performed.
**
** @param ptr: Pointer to memory to free (or NULL)
**
** Thread safety: This function is thread-safe (uses mutex internally)
*/

void	free(void *ptr);

/*
** realloc()
**
** Changes the size of the memory block pointed to by ptr to size bytes.
** The contents will be unchanged in the range from the start of the region
** up to the minimum of the old and new sizes. If the new size is larger,
** the added memory is not initialized.
**
** If ptr is NULL, equivalent to malloc(size).
** If size is 0, equivalent to free(ptr) and returns NULL.
**
** @param ptr: Pointer to existing allocation (or NULL)
** @param size: New size in bytes
** @return: Pointer to reallocated memory, or NULL on failure
**
** Thread safety: This function is thread-safe (uses mutex internally)
*/

void	*realloc(void *ptr, size_t size);

/*
** show_alloc_mem()
**
** Displays a visual representation of all allocated memory zones.
** Output format (sorted by address):
**
** TINY : 0xA0000
** 0xA0020 - 0xA004A : 42 bytes
** 0xA006A - 0xA00BE : 84 bytes
** SMALL : 0xAD000
** 0xAD020 - 0xADEAD : 3725 bytes
** LARGE : 0xB0000
** 0xB0020 - 0xBBEEF : 48847 bytes
** Total : 52698 bytes
**
** Context: This is a debugging function required by the subject.
** It provides visibility into memory allocation state.
*/

void	show_alloc_mem(void);

#endif

