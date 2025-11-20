/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malloc                                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18                               #+#    #+#             */
/*   Updated: 2025/01/18                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "zone.h"
#include "block.h"
#include "alloc_hdr.h"
#include "utils.h"
#include "fit.h"
#include "align.h"
#include <stddef.h>
#include <string.h>

/*
** ft_free_list_remove()
**
** Removes a block from the zone's free list.
** Helper function for malloc when allocating a block.
*/

static void	ft_free_list_remove(ft_zone_t *zone, ft_block_t *block)
{
	if (block->prev_free)
		block->prev_free->next_free = block->next_free;
	else
		zone->free_head = block->next_free;
	if (block->next_free)
		block->next_free->prev_free = block->prev_free;
	block->prev_free = NULL;
	block->next_free = NULL;
}

/*
** ft_free_list_add()
**
** Adds a block to the front of the zone's free list.
** Helper function for free() when returning a block to the free pool.
*/

static void	ft_free_list_add(ft_zone_t *zone, ft_block_t *block)
{
	block->prev_free = NULL;
	block->next_free = zone->free_head;
	if (zone->free_head)
		zone->free_head->prev_free = block;
	zone->free_head = block;
}

/*
** ft_allocate_from_block()
**
** Allocates memory from a free block.
** Splits the block if there's excess space, marks it as allocated,
** sets up the allocation header, and returns the user pointer.
*/

static void	*ft_allocate_from_block(ft_zone_t *zone, ft_block_t *block,
	size_t alloc_size, size_t user_size)
{
	ft_block_t		*remainder;
	ft_alloc_hdr_t	*hdr;
	void			*user_ptr;

	ft_free_list_remove(zone, block);
	remainder = ft_block_split(block, alloc_size);
	if (remainder)
		ft_free_list_add(zone, remainder);
	block->is_free = 0;
	hdr = (ft_alloc_hdr_t *)ft_block_data_ptr(block);
	hdr->magic = FT_ALLOC_MAGIC;
	hdr->size = user_size;
	hdr->block = block;
	hdr->zone = zone;
	user_ptr = ft_alloc_hdr_to_ptr(hdr);
	zone->used_size += user_size;
	zone->block_count++;
	return (user_ptr);
}

/*
** malloc()
**
** Main allocation function.
** Algorithm:
** 1. Determine zone type based on size
** 2. Calculate total size needed (including all headers and alignment)
** 3. For TINY/SMALL: try to find existing free block, else create new zone
** 4. For LARGE: always create dedicated zone
** 5. Allocate from block and return user pointer
*/

void	*malloc(size_t size)
{
	uint8_t		type;
	size_t		alloc_size;
	ft_zone_t	*zone;
	ft_block_t	*block;

	if (size == 0)
		return (NULL);
	type = ft_zone_get_type(size);
	alloc_size = ft_calculate_alloc_size(size); // block header + alloc header + size
	if (type == FT_ZONE_LARGE)
	{
		zone = ft_zone_create(type, alloc_size);
		if (!zone)
			return (NULL);
		block = zone->first_block;
		return (ft_allocate_from_block(zone, block, alloc_size, size));
	}
	block = ft_first_fit(type, alloc_size, &zone);
	if (!block)
	{
		zone = ft_zone_create(type, 0);
		if (!zone)
			return (NULL);
		block = zone->first_block;
	}
	return (ft_allocate_from_block(zone, block, alloc_size, size));
}

/*
** ft_coalesce_blocks()
**
** After freeing a block, attempt to merge it with adjacent free blocks
** to reduce external fragmentation.
*/

static void	ft_coalesce_blocks(ft_zone_t *zone, ft_block_t *block)
{
	ft_block_t	*next;

	if (block->next && ft_block_can_merge(block, block->next))
	{
		next = block->next;
		ft_free_list_remove(zone, next); // must remove since it's independent
		ft_block_merge(block, next);
	}
	if (block->prev && ft_block_can_merge(block->prev, block))
	{
		ft_free_list_remove(zone, block); // must remove since it's independent
		ft_block_merge(block->prev, block);
	}
}

/*
** free()
**
** Frees previously allocated memory.
** Algorithm:
** 1. Validate pointer using allocation header magic number
** 2. Mark block as free and add to free list
** 3. Coalesce with adjacent free blocks
** 4. If zone becomes empty, unmap it (all zone types)
*/

void	free(void *ptr)
{
	ft_alloc_hdr_t	*hdr;
	ft_block_t		*block;
	ft_zone_t		*zone;

	if (!ptr)
		return ;
	hdr = ft_alloc_hdr_from_ptr(ptr);
	if (!ft_alloc_hdr_is_valid(hdr))
		return ;
	hdr->magic = 0;
	block = (ft_block_t *)hdr->block;
	zone = (ft_zone_t *)hdr->zone;
	block->is_free = 1;
	ft_free_list_add(zone, block);
	zone->used_size -= hdr->size;
	zone->block_count--;
	ft_coalesce_blocks(zone, block);
	if (zone->block_count == 0)
		ft_zone_remove(zone);
}

/*
** ft_try_extend_in_place()
**
** Attempts to extend an allocation in place by merging with the next free block.
** This avoids expensive malloc + memcpy + free operations when growing buffers.
**
** @param block: Current block to extend
** @param zone: Zone containing the block
** @param needed_size: Total size needed for the new allocation
** @return: 1 if extension succeeded, 0 if not possible
**
** Context: Called by realloc when growing an allocation. Checks if the next
** block in address order is free and large enough to accommodate the growth.
*/

static int	ft_try_extend_in_place(ft_block_t *block, ft_zone_t *zone,
	size_t needed_size)
{
	ft_block_t	*next;
	size_t		available;
	ft_block_t	*remainder;

	next = block->next;
	if (!next || !next->is_free)
		return (0);
	available = block->size + next->size;
	if (available < needed_size)
		return (0);
	ft_free_list_remove(zone, next);
	ft_block_merge(block, next);
	remainder = ft_block_split(block, needed_size);
	if (remainder)
		ft_free_list_add(zone, remainder);
	return (1);
}

/*
** realloc()
**
** Changes the size of an allocation.
** Algorithm:
** 1. Handle special cases (NULL ptr, size 0)
** 2. Validate existing allocation
** 3. If new size fits in current block, just update header
** 4. Try to extend in place by merging with next free block
** 5. If that fails: allocate new block, copy data, free old block
*/

void	*realloc(void *ptr, size_t size)
{
	ft_alloc_hdr_t	*hdr;
	void			*new_ptr;
	size_t			copy_size;

	if (!ptr)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	hdr = ft_alloc_hdr_from_ptr(ptr);
	if (!ft_alloc_hdr_is_valid(hdr))
		return (NULL);
	if (hdr->size >= size)
	{
		hdr->size = size;
		return (ptr);
	}
	if (ft_try_extend_in_place((ft_block_t *)hdr->block,
		(ft_zone_t *)hdr->zone, ft_calculate_alloc_size(size)))
	{
		((ft_zone_t *)hdr->zone)->used_size += (size - hdr->size);
		hdr->size = size;
		return (ptr);
	}
	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	copy_size = hdr->size < size ? hdr->size : size;
	memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return (new_ptr);
}

