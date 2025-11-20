/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malloc                                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18                               #+#    #+#             */
/*   Updated: 2025/01/18                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block.h"
#include "zone.h"
#include <stddef.h>
#include <stdint.h>

/*
** ft_block_init()
**
** Initializes a new block at the given address with the specified size.
** The block starts as free with all pointers NULL.
*/

ft_block_t	*ft_block_init(void *addr, size_t size, void *zone)
{
	ft_block_t	*block;

	block = (ft_block_t *)addr;
	block->size = size;
	block->is_free = 1;
	block->prev = NULL;
	block->next = NULL;
	block->prev_free = NULL;
	block->next_free = NULL;
	block->zone = zone;
	return (block);
}

/*
** ft_block_split()
**
** Splits a block if there's enough space remaining after allocation.
** The original block is shrunk to 'size', and a new free block is created
** for the remainder.
**
** Returns NULL if the remainder would be too small to be useful.
*/

ft_block_t	*ft_block_split(ft_block_t *block, size_t size)
{
	ft_block_t	*remainder;
	size_t		remainder_size;
	uint8_t		*remainder_addr;

	if (block->size < size + FT_MIN_BLOCK_SIZE)
		return (NULL);
	remainder_size = block->size - size;
	remainder_addr = (uint8_t *)block + size;
	remainder = ft_block_init(remainder_addr, remainder_size, block->zone);
	remainder->prev = block;
	remainder->next = block->next;
	if (block->next)
		block->next->prev = remainder;
	block->next = remainder;
	block->size = size;
	return (remainder);
}

/*
** ft_block_can_merge()
**
** Checks if two blocks are adjacent in memory and both free.
** Blocks can only be merged if 'a' immediately precedes 'b' in address space.
*/

int	ft_block_can_merge(ft_block_t *a, ft_block_t *b)
{
	uint8_t	*end_of_a;

	if (!a || !b)
		return (0);
	if (!a->is_free || !b->is_free)
		return (0);
	if (a->zone != b->zone)
		return (0);
	end_of_a = (uint8_t *)a + a->size;
	return (end_of_a == (uint8_t *)b);
}

/*
** ft_block_merge()
**
** Merges block 'b' into block 'a'.
** Block 'a' is extended and block 'b' is absorbed (removed from lists).
** Assumes ft_block_can_merge(a, b) has already been checked.
*/

void	ft_block_merge(ft_block_t *a, ft_block_t *b)
{
	a->size += b->size;
	a->next = b->next;
	if (b->next)
		b->next->prev = a;
}

