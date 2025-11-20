/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malloc                                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18                               #+#    #+#             */
/*   Updated: 2025/01/18                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fit.h"
#include "zone.h"
#include "block.h"
#include <stddef.h>

/*
** ft_first_fit()
**
** Implements first-fit allocation strategy.
** Iterates through all zones of the given type, searching each zone's
** free list for a block large enough to satisfy the request.
**
** Returns the first suitable block found, setting out_zone to point
** to the zone containing that block.
*/

ft_block_t	*ft_first_fit(uint8_t type, size_t size, ft_zone_t **out_zone)
{
	ft_zone_t	*zone;
	ft_block_t	*block;

	zone = *ft_zone_get_list(type);
	while (zone)
	{
		block = zone->free_head;
		while (block)
		{
			if (block->is_free && block->size >= size)
			{
				*out_zone = zone;
				return (block);
			}
			block = block->next_free;
		}
		zone = zone->next;
	}
	*out_zone = NULL;
	return (NULL);
}

