/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malloc                                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18                               #+#    #+#             */
/*   Updated: 2025/01/18                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zone.h"
#include "block.h"
#include "utils.h"
#include "align.h"
#include <sys/mman.h>
#include <stddef.h>

/*
** Global zone manager - tracks all zones by type
*/

ft_zone_mgr_t	g_zone_mgr = {NULL, NULL, NULL};

/*
** ft_zone_get_type()
**
** Determines zone type based on allocation size.
** Categorizes into TINY (1-128), SMALL (129-1024), or LARGE (1025+).
*/

uint8_t	ft_zone_get_type(size_t size)
{
	if (size <= FT_TINY_MAX)
		return (FT_ZONE_TINY);
	else if (size <= FT_SMALL_MAX)
		return (FT_ZONE_SMALL);
	else
		return (FT_ZONE_LARGE);
}

/*
** ft_zone_get_list()
**
** Returns pointer to the appropriate zone list head for the given type.
*/

ft_zone_t	**ft_zone_get_list(uint8_t type)
{
	if (type == FT_ZONE_TINY)
		return (&g_zone_mgr.tiny_zones);
	else if (type == FT_ZONE_SMALL)
		return (&g_zone_mgr.small_zones);
	else
		return (&g_zone_mgr.large_zones);
}

/*
** ft_zone_add()
**
** Adds a zone to the front of the appropriate list.
** Zones are added to the front for O(1) insertion.
** This is a static helper function, called automatically by ft_zone_create().
*/

static void	ft_zone_add(ft_zone_t *zone)
{
	ft_zone_t	**list_head;

	list_head = ft_zone_get_list(zone->type);
	zone->next = *list_head;
	zone->prev = NULL;
	if (*list_head)
		(*list_head)->prev = zone;
	*list_head = zone;
}

/*
** ft_zone_init_block_list()
**
** Initializes the first block in a newly created zone.
** The entire usable space becomes one large free block.
*/

static void	ft_zone_init_block_list(ft_zone_t *zone)
{
	size_t		usable_size;
	void		*block_addr;
	ft_block_t	*block;

	block_addr = (uint8_t *)zone + FT_ZONE_HDR_SIZE;
	block_addr = ft_align_up_ptr(block_addr, FT_ALIGN_SIZE);
	usable_size = zone->total_size - ((uint8_t *)block_addr - (uint8_t *)zone);
	block = ft_block_init(block_addr, usable_size, zone);
	zone->first_block = block;
	zone->free_head = block;
}

/*
** ft_zone_create()
**
** Creates a new zone using mmap().
** For TINY/SMALL: allocates multiple pages
** For LARGE: allocates exact size needed (rounded to pagesize)
*/

ft_zone_t	*ft_zone_create(uint8_t type, size_t size)
{
	ft_zone_t	*zone;
	size_t		total_size;
	void		*addr;

	total_size = ft_calculate_zone_size(type, size);
	addr = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED)
		return (NULL);
	zone = (ft_zone_t *)addr;
	zone->type = type;
	zone->total_size = total_size;
	zone->used_size = 0;
	zone->block_count = 0;
	zone->first_block = NULL;
	zone->free_head = NULL;
	zone->prev = NULL;
	zone->next = NULL;
	zone->start_addr = addr;
	zone->end_addr = (uint8_t *)addr + total_size;
	ft_zone_init_block_list(zone);
	ft_zone_add(zone);
	return (zone);
}

/*
** ft_zone_remove()
**
** Removes a zone from its list and unmaps it.
** Used when a LARGE zone is freed or potentially for empty TINY/SMALL zones.
*/

void	ft_zone_remove(ft_zone_t *zone)
{
	ft_zone_t	**list_head;

	list_head = ft_zone_get_list(zone->type);
	if (zone->prev)
		zone->prev->next = zone->next;
	else
		*list_head = zone->next;
	if (zone->next)
		zone->next->prev = zone->prev;
	munmap(zone, zone->total_size);
}

/*
** ft_zone_find_free_block()
**
** Searches a single zone for a free block large enough.
** Returns NULL if no suitable block is found.
*/

ft_block_t	*ft_zone_find_free_block(ft_zone_t *zone, size_t size)
{
	ft_block_t	*block;

	block = zone->free_head;
	while (block)
	{
		if (block->is_free && block->size >= size)
			return (block);
		block = block->next_free;
	}
	return (NULL);
}

