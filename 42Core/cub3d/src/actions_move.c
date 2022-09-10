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

// static int can_move(t_config *conf, t_point p, t_vector vec)
// {
// 	int	i;

// 	i = 0;
// 	while (i < conf->pov->mag)
// 	{
// 		vec.mag = i;
// 		p = add_vec(p, vec);
// 		if (conf->map[(int)(p.y / TILE_SIZE)][(int)(p.x / TILE_SIZE)] != '1')
// 			return (-1);
// 		i++;
// 	}
// 	return (1);
// }

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
	{
		move_up(conf);
		conf->pov->n_down = 1;
	}
	else if (key_code == KEY_S)
	{
		move_down(conf);
		conf->pov->s_down = 1;
	}
	else if (key_code == KEY_D)
	{
		move_right(conf);
		conf->pov->e_down = 1;
	}
	else if (key_code == KEY_A)
	{
		move_left(conf);
		conf->pov->w_down = 1;
	}
}

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
	conf->pov->angle = (conf->pov->angle - r_angle(MAGNITUDE_ANGLE));
	if (conf->pov->angle < 0)
		conf->pov->angle = r_angle(360) + r_angle(360);
	if (conf->pov->angle > r_angle(360))
		conf->pov->angle = conf->pov->angle - r_angle(360);
}

static void	rotate_left(t_config *conf)
{
	conf->pov->angle = (conf->pov->angle + r_angle(MAGNITUDE_ANGLE));
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

static void	paint_window(t_config *conf)
{
	t_image		*img_map;
	t_image		*img_game;

	img_game = new_image(conf->img_game->win, conf->img_game->h,
			conf->img_game->w);
	mlx_destroy_image(conf->img_game->win->mlx_ptr, conf->img_game->img_ptr);
	free(conf->img_game);
	conf->img_game = img_game;
	raycasting3d(conf);
	mlx_put_image_to_window(conf->img_game->win->mlx_ptr,
		conf->img_game->win->win_ptr, img_game->img_ptr, 0, 0);
	img_map = new_image(conf->img_map->win, conf->img_map->h, conf->img_map->w);
	mlx_destroy_image(conf->img_map->win->mlx_ptr, conf->img_map->img_ptr);
	free(conf->img_map);
	conf->img_map = img_map;
	paint_map(conf);
	mlx_put_image_to_window(conf->img_map->win->mlx_ptr,
		conf->img_map->win->win_ptr, img_map->img_ptr, 0, 0);
}


int	update(t_config *conf)
{
	static int fps = 0;

	if (conf->pov->w_down || conf->pov->n_down || conf->pov->s_down || conf->pov->e_down)
	{
		printf("%c", conf->pov->w_down ? '<' : ' ');
		printf("%c", conf->pov->n_down ? 'N' : ' ');
		printf("%c", conf->pov->s_down ? 'v' : ' ');
		printf("%c", conf->pov->e_down ? '>' : ' ');
		printf("\n");
	}
	if (fps++ == 5) {
		if (conf->pov->n_down)
			move(KEY_W, conf);
		else if (conf->pov->s_down)
			move(KEY_S, conf);
		if (conf->pov->w_down)
			rotate(KEY_LEFT, conf);
		else if (conf->pov->e_down)
			rotate(KEY_RIGHT, conf);
		if (conf->pov->n_down || conf->pov->s_down || conf->pov->e_down || conf->pov->w_down)
			paint_window(conf);
		fps = 0;
	}
	return (0);
}
