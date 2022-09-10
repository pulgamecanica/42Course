/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- & arosado- <marvin@42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:00:00 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:15:00 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	look_up(t_config *conf)
{
	if (conf->pov->center >= WIN_H - TILE_SIZE)
		return ;
	else
		conf->pov->center += 5;
}

static void	look_down(t_config *conf)
{
	if (conf->pov->center <= 10)
		return ;
	else
		conf->pov->center -= 5;
}

static void	rotate_right(t_config *conf)
{
	conf->pov->angle = (conf->pov->angle - r_angle(5));
	if (conf->pov->angle < 0)
		conf->pov->angle = r_angle(360) + r_angle(360);
	if (conf->pov->angle > r_angle(360))
		conf->pov->angle = conf->pov->angle - r_angle(360);
}

static void	rotate_left(t_config *conf)
{
	conf->pov->angle = (conf->pov->angle + r_angle(5));
	if (conf->pov->angle > r_angle(360))
		conf->pov->angle = conf->pov->angle - r_angle(360);
	if (conf->pov->angle < 0)
		conf->pov->angle = r_angle(360) + r_angle(360);
}

void	rotate(int key_code, t_config *conf)
{
	if (key_code == KEY_LEFT || key_code == KEY_Q)
		rotate_left(conf);
	else if (key_code == KEY_RIGHT || key_code == KEY_E)
		rotate_right(conf);
	else if (key_code == KEY_DOWN)
		look_down(conf);
	else if (key_code == KEY_UP)
		look_up(conf);
}
