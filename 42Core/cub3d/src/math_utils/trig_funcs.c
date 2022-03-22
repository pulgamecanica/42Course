/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trig_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- & arosado- <marvin@42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:08:23 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:09:57 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	fcos(int angle)
{
	static double	fcos[360 * WIN_W / 60];
	int				i;

	if (angle < r_angle(0) || angle >= r_angle(360))
		return (0.0);
	if (fcos[r_angle(0)] > 1.0)
		return (fcos[angle]);
	i = 0;
	while (i < r_angle(360))
	{
		fcos[i] = cos(i * PI / r_angle(180)) + 0.00001;
		i++;
	}
	return (fcos[angle]);
}

double	fsin(int angle)
{
	static double	fsin[360 * WIN_W / 60];
	int				i;

	if (angle < r_angle(0) || angle >= r_angle(360))
		return (0.0);
	if (fsin[r_angle(90)] > 1.0)
		return (fsin[angle]);
	i = 0;
	while (i < r_angle(360))
	{
		fsin[i] = sin(i * PI / r_angle(180)) + 0.00001;
		i++;
	}
	return (fsin[angle]);
}

double	ftan(int angle)
{
	static double	ftan[360 * WIN_W / 60];
	int				i;

	if (angle < r_angle(0) || angle >= r_angle(360))
		return (0.0);
	if (ftan[r_angle(45)] > 1.0)
		return (ftan[angle]);
	i = 0;
	while (i < r_angle(360))
	{
		ftan[i] = tan(i * PI / r_angle(180)) + 0.00001;
		i++;
	}
	return (ftan[angle]);
}

double	norm(t_config *conf, int xx, int yy)
{
	return (sqrt(pow((conf->pov->p.x / MAP_SCALE) - (xx), 2)
			+ pow((conf->pov->p.y / MAP_SCALE) - (yy), 2))
		<= sqrt(MAP_TILE_SIZE));
}
