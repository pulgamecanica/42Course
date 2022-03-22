/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 08:24:28 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/17 08:18:49 by dareias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	edge_collide(t_config *conf, t_point p)
{
	if (p.x < 0 || p.y < 0 || p.x / TILE_SIZE >= conf->width
		|| p.y / TILE_SIZE >= conf->height)
		return (1);
	return (0);
}

static int	wall_collide(t_config *conf, t_point p, int angle)
{
	int	my_y;
	int	my_x;

	if (angle < r_angle(180))
		my_y = ceil((p.y)) / (TILE_SIZE);
	else
		my_y = floor((p.y)) / (TILE_SIZE);
	if (angle > r_angle(90) && angle < r_angle(270))
		my_x = ceil((p.x)) / (TILE_SIZE);
	else
		my_x = floor((p.x)) / (TILE_SIZE);
	if (conf->map[my_y][my_x] == '1')
		return (1);
	return (0);
}

int	collide(t_config *conf, t_point p, int angle)
{	
	return (edge_collide(conf, p) || wall_collide(conf, p, angle));
}
