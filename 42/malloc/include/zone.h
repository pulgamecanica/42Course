#ifndef ZONE_H
# define ZONE_H

# include <stddef.h>
# include <stdint.h>
# include "block.h"

/*
** Zone types - categorizes allocations by size
**
** TINY:  1 - 128 bytes    (stored in multi-page zones)
** SMALL: 129 - 1024 bytes (stored in multi-page zones)
** LARGE: 1025+ bytes      (each allocation gets its own mmap)
**
** These are defines rather than an enum to allow use in preprocessor
** conditionals and to match the subject's terminology.
*/

# define FT_ZONE_TINY  0
# define FT_ZONE_SMALL 1
# define FT_ZONE_LARGE 2

/*
** Size thresholds for zone classification
*/

# define FT_TINY_MAX   128
# define FT_SMALL_MAX  1024

/*
** Zone capacities - number of pages per zone type
**
** These values balance:
** - Reducing mmap() calls (subject requirement)
** - Memory waste from unused pre-allocated space
** - Ensuring at least 100 allocations per zone (subject requirement)
**
** Remember: 1KB = 128bytes
** Calculation example for TINY (assuming 4KB pages -> 512bytes):
** - 4 pages = 16KB total
** - Zone header ~64 bytes
** - Each allocation: ~64 bytes (block + alloc hdr + 128 bytes data)
** - Capacity: ~250 allocations (exceeds 100 minimum)
*/

# define FT_TINY_ZONE_PAGES  4
# define FT_SMALL_ZONE_PAGES 16

/*
** ft_zone_t - Memory Zone
**
** A zone is a pre-allocated region of memory that holds multiple blocks.
** TINY and SMALL allocations are grouped into zones to reduce mmap() calls.
** LARGE allocations each get their own zone (single allocation per zone).
**
** Zones are organized into three linked lists (one per type) in the global
** zone manager. This allows show_alloc_mem() to easily iterate by type.
**
** Memory layout of a zone:
** [ft_zone_t header][ft_block_t][alloc_hdr][data...][ft_block_t][...]
*/

typedef struct s_zone
{
	uint8_t			type;					/* FT_ZONE_TINY, FT_ZONE_SMALL, or FT_ZONE_LARGE */
	size_t			total_size;		/* Total size of this zone (from mmap) */
	size_t			used_size;		/* Bytes currently allocated to users */
	size_t			block_count;	/* Number of allocations in this zone */

	/* Block tracking */
	ft_block_t		*first_block;	/* First block in address order */
	ft_block_t		*free_head;		/* Head of free block list (not ordered) */

	/* Zone list linkage */
	struct s_zone	*prev;			/* Previous zone of same type */
	struct s_zone	*next;			/* Next zone of same type */

	/* Metadata for debugging */
	void			*start_addr;	/* Start of zone memory (for validation) */
	void			*end_addr;		/* End of zone memory (for validation) */
}	t_zone;

typedef t_zone	ft_zone_t;

/*
** FT_ZONE_HDR_SIZE - Size of zone header (aligned)
*/

# define FT_ZONE_HDR_SIZE (sizeof(ft_zone_t))

/*
** ft_zone_mgr_t - Global Zone Manager
**
** Maintains three separate linked lists for each zone type.
** This singleton structure is the root of our memory allocation system.
**
** Design decision: Separate lists by type allow:
** - Easy iteration for show_alloc_mem() (print TINY, then SMALL, then LARGE)
** - Type-specific optimizations
** - Clear visualization of memory organization
*/

typedef struct s_zone_mgr
{
	ft_zone_t	*tiny_zones;	/* Linked list of TINY zones */
	ft_zone_t	*small_zones;	/* Linked list of SMALL zones */
	ft_zone_t	*large_zones;	/* Linked list of LARGE zones */
}	t_zone_mgr;

typedef t_zone_mgr	ft_zone_mgr_t;

/*
** Global zone manager (defined in zone.c)
**
** This is one of two allowed global variables (subject allows one for
** allocation management and one for thread safety).
*/

extern ft_zone_mgr_t	g_zone_mgr;

/*
** Zone management functions (implemented in zone.c)
*/

/*
** ft_zone_get_type()
**
** Determines which zone type should be used for a given allocation size.
**
** @param size: User-requested allocation size
** @return: FT_ZONE_TINY, FT_ZONE_SMALL, or FT_ZONE_LARGE
**
** Context: Called at the beginning of malloc to route the allocation
** to the appropriate zone type.
*/

uint8_t		ft_zone_get_type(size_t size);

/*
** ft_zone_create()
**
** Creates a new zone using mmap() and automatically registers it in the
** global zone manager.
** For TINY/SMALL, allocates multiple pages.
** For LARGE, allocates exact size needed.
**
** @param type: Zone type (FT_ZONE_TINY, FT_ZONE_SMALL, FT_ZONE_LARGE)
** @param size: For LARGE zones, the specific size; ignored for TINY/SMALL
** @return: Pointer to new zone (already added to manager), or NULL on failure
**
** Context: Called when no existing zone has space for an allocation.
** This is where mmap() is invoked. The zone is automatically added to the
** appropriate list in g_zone_mgr before returning.
*/

ft_zone_t	*ft_zone_create(uint8_t type, size_t size);

/*
** ft_zone_find_free_block()
**
** Searches for a free block in a zone that can fit the requested size.
** Uses first-fit(or choosen) strategy: returns the first block large enough.
**
** @param zone: Zone to search
** @param size: Minimum block size needed
** @return: Pointer to suitable block, or NULL if none found
**
** Context: Called during allocation to find available space within
** existing zones before creating new zones.
*/

ft_block_t	*ft_zone_find_free_block(ft_zone_t *zone, size_t size);

/*
** ft_zone_remove()
**
** Removes a zone from the global zone manager and frees it with munmap().
**
** @param zone: Zone to remove
**
** Context: Called when a zone becomes empty and can be returned to the OS.
** Currently only happens for LARGE zones immediately after free().
*/

void		ft_zone_remove(ft_zone_t *zone);

/*
** ft_zone_get_list()
**
** Gets the appropriate zone list head for a given type.
**
** @param type: Zone type
** @return: Pointer to the list head for that type
**
** Context: Helper for iterating zones by type, used by show_alloc_mem().
*/

ft_zone_t	**ft_zone_get_list(uint8_t type);

#endif

