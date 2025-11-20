#ifndef FIT_H
# define FIT_H

# include "zone.h"
# include "block.h"

/*
** Allocation strategy functions.
**
** These implement the "fit" algorithms that decide which free block
** to use when multiple options are available.
**
** First-fit: Use the first block that's large enough
** - Simple and fast
** - Can lead to fragmentation at the beginning of the zone
** - Good enough for a pedagogical implementation
**
** Other strategies (not implemented, but could be added):
** - Best-fit: Find the smallest block that fits (minimizes waste)
** - Worst-fit: Find the largest block (keeps large blocks available)
*/

/*
** ft_first_fit()
**
** Implements first-fit allocation strategy.
** Searches zones of the given type for the first free block large enough.
**
** @param type: Zone type to search (FT_ZONE_TINY, FT_ZONE_SMALL)
** @param size: Minimum block size needed
** @param out_zone: Output parameter - pointer to zone containing the block
** @return: Pointer to suitable free block, or NULL if none found
**
** Context: Called by malloc to find available space before creating new zones.
*/

ft_block_t	*ft_first_fit(uint8_t type, size_t size, ft_zone_t **out_zone);

#endif

