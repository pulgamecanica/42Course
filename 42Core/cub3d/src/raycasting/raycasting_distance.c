/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_distance.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- & arosado- <marvin@42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 08:24:28 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/02 08:24:33 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	nearest_horizontal(t_config *conf, int a)
{
	t_point	intersection;
	t_point	p_const;

	if (a < r_angle(180))
		intersection.y = floor(conf->pov->p.y / TILE_SIZE) * TILE_SIZE - 1;
	else
	intersection.y = floor(conf->pov->p.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
	intersection.x = conf->pov->p.x
		+ (conf->pov->p.y - intersection.y) / ftan(a);
	p_const = horizontal_step(a);
	while (!collide(conf, intersection, a))
		intersection = add_point(intersection, p_const);
	if (intersection.x < 0 || intersection.y < 0 || intersection.x / TILE_SIZE
		> conf->width || intersection.y / TILE_SIZE > conf->height)
		return (INT_MAX);
	return (fabs(fabs(conf->pov->p.y - intersection.y) / fsin(a)));
}

double	nearest_vertical(t_config *conf, int a)
{
	t_point	intersection;
	t_point	p_const;

	if (a > r_angle(90) && a < r_angle(270))
		intersection.x = floor(conf->pov->p.x / TILE_SIZE) * (TILE_SIZE) - 1;
	else
		intersection.x = floor(conf->pov->p.x / TILE_SIZE)
			* TILE_SIZE + TILE_SIZE;
	intersection.y = conf->pov->p.y
		+ ((conf->pov->p.x - intersection.x) * ftan(a));
	p_const = vertical_step(a);
	while (!collide(conf, intersection, a))
		intersection = add_point(intersection, p_const);
	if (intersection.x < 0 || intersection.y < 0 || intersection.x / TILE_SIZE
		> conf->width || intersection.y / TILE_SIZE > conf->height)
		return (INT_MAX);
	return (fabs(fabs(conf->pov->p.x - intersection.x) / fcos(a)));
}

double	magic_distance(t_config *conf, int ray_angle)
{
	int		a;
	double	ph;
	double	pv;

	a = conf->pov->angle + ray_angle;
	if (a < 0)
		a = r_angle(360) + a;
	if (a >= r_angle(360))
		a = a - r_angle(360);
	ph = nearest_horizontal(conf, a);
	pv = nearest_vertical(conf, a);
	if (ph < pv)
		return (ph);
	else
		return (pv);
}
