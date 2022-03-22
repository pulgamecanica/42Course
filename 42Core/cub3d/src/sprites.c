/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- & arosado- <marvin@42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 08:24:28 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/02 08:24:33 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	bubble_sort_sprites(int *sprite_order,
	double *sprite_distance, int sprite_ammount)
{
	int			i;
	int			j;
	int			temp;

	i = 0;
	j = 0;
	while (i < sprite_ammount - 1)
	{
		j = 0;
		while (j < sprite_ammount - i - 1)
		{
			if (sprite_distance[sprite_order[j]]
				< sprite_distance[sprite_order[j + 1]])
			{
				temp = sprite_order[j];
				sprite_order[j] = sprite_order[j + 1];
				sprite_order[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

int	*sort_sprites(t_sprite *sprites, int px, int py)
{
	int		*sprite_order;
	double	sprite_distance[SPRITE_AMMOUNT];
	int		i;
	int		x;

	i = 0;
	sprite_order = malloc(sizeof(int) * SPRITE_AMMOUNT);
	if (!sprite_order)
		return ((void *)0);
	while (i < SPRITE_AMMOUNT)
	{
		sprite_order[i] = i;
		sprite_distance[i] = ((px - sprites[i].x) * (px - sprites[i].x)
				+ (py - sprites[i].y) * (py - sprites[i].y));
		i++;
	}
	bubble_sort_sprites(sprite_order, sprite_distance, SPRITE_AMMOUNT);
	x = -1;
	while (++x < SPRITE_AMMOUNT)
	{
		printf("Sprite order: %i\n", sprite_order[x]);
	}
	free(sprite_order);
	return (sprite_order);
}
