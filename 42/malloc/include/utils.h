#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include <stdint.h>
# include <unistd.h>

/*
** ft_pagesize()
**
** Returns the system page size.
** Uses sysconf(_SC_PAGESIZE) on Linux, getpagesize() on macOS.
** Caches the result since page size doesn't change during execution.
**
** @return: System page size in bytes (typically 4096)
**
** Context: The subject requires zone sizes to be multiples of the page size.
** mmap() also works with page-aligned sizes for optimal performance.
*/

size_t	ft_pagesize(void);

/*
** ft_calculate_zone_size()
**
** Calculates the total size needed for a zone based on type.
** For TINY/SMALL: returns multiple of page size.
** For LARGE: returns size rounded up to page size.
**
** @param type: Zone type (FT_ZONE_TINY, FT_ZONE_SMALL, FT_ZONE_LARGE)
** @param request_size: For LARGE, the requested allocation size (ignored for TINY & SMALL)
** @return: Total zone size in bytes
**
** Context: Used when creating zones to determine mmap() size parameter.
*/

size_t	ft_calculate_zone_size(uint8_t type, size_t request_size);

/*
** ft_calculate_alloc_size()
**
** Calculates total size needed for an allocation including all overhead.
** Includes: block header + allocation header + user size + alignment padding
**
** @param user_size: Size requested by user
** @return: Total size needed (aligned)
**
** Context: Used by malloc to determine how much space is actually needed
** from a block, accounting for all metadata and alignment requirements.
*/

size_t	ft_calculate_alloc_size(size_t user_size);

#endif

