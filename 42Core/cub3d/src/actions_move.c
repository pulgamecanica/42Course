/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_move.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- & arosado- <marvin@42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:08:56 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:10:30 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	move_right(t_config *conf)
{
	t_point	p;

	p.x = conf->pov->p.x;
	p.y = conf->pov->p.y;
	p = add_vec(p, vec(conf->pov->angle - r_angle(90), conf->pov->mag));
	if (p.x > conf->pov->width || p.x < 0 || p.y > conf->pov->height || p.y < 0)
		return ;
	if (conf->map[(int)(p.y / TILE_SIZE)][(int)(p.x / TILE_SIZE)] != '1')
	{
		conf->pov->p.y = round(p.y);
		conf->pov->p.x = round(p.x);
	}
}

static void	move_left(t_config *conf)
{
	t_point	p;

	p.x = conf->pov->p.x;
	p.y = conf->pov->p.y;
	p = add_vec(p, vec(conf->pov->angle + r_angle(90), conf->pov->mag));
	if (p.x > conf->pov->width || p.x < 0 || p.y > conf->pov->height || p.y < 0)
		return ;
	if (conf->map[(int)(p.y / TILE_SIZE)][(int)(p.x / TILE_SIZE)] != '1')
	{
		conf->pov->p.y = round(p.y);
		conf->pov->p.x = round(p.x);
	}
}

static void	move_up(t_config *conf)
{
	t_point	p;

	p.x = conf->pov->p.x;
	p.y = conf->pov->p.y;
	p = add_vec(p, vec(conf->pov->angle, conf->pov->mag));
	if (p.x > conf->pov->width || p.x < 0 || p.y > conf->pov->height || p.y < 0)
		return ;
	if (conf->map[(int)(p.y / TILE_SIZE)][(int)(p.x / TILE_SIZE)] != '1')
	{
		conf->pov->p.y = round(p.y);
		conf->pov->p.x = round(p.x);
	}
}

static void	move_down(t_config *conf)
{
	t_point	p;

	p.x = conf->pov->p.x;
	p.y = conf->pov->p.y;
	p = add_vec(p, vec(conf->pov->angle, conf->pov->mag * -1));
	if (p.x > conf->pov->width || p.x < 0 || p.y > conf->pov->height || p.y < 0)
		return ;
	if (conf->map[(int)(p.y / TILE_SIZE)][(int)(p.x / TILE_SIZE)] != '1')
	{
		conf->pov->p.y = round(p.y);
		conf->pov->p.x = round(p.x);
	}
}

void	move(int key_code, t_config *conf)
{
	if (key_code == KEY_W)
		move_up(conf);
	else if (key_code == KEY_S)
		move_down(conf);
	else if (key_code == KEY_D)
		move_right(conf);
	else if (key_code == KEY_A)
		move_left(conf);
}
