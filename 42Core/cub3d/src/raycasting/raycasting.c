/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 08:24:28 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/20 15:01:39 by dareias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_image	*correct_texture(t_config *conf, int i)
{
	if (i == 1)
		return (conf->walls.no_textu);
	else if (i == 2)
		return (conf->walls.so_textu);
	else if (i == 3)
		return (conf->walls.we_textu);
	else
		return (conf->walls.ea_textu);
}

static int	shade(double distance, int color, t_config *conf)
{
	double	factor;
	int		div;
	int		r;
	int		g;
	int		b;

	(void)div;
	if (conf->pov->width < conf->pov->height)
		div = conf->pov->width;
	else
		div = conf->pov->height;
	factor = fabs((distance / conf->pov->width) - 1);
	if (factor > 1)
		factor = 1;
	r = get_r(color) * factor;
	g = get_g(color) * factor;
	b = get_b(color) * factor;
	if (r < 30)
		r = 30;
	if (g < 30)
		g = 30;
	if (b < 30)
		b = 30;
	return (gen_trgb(0, r, g, b));
}

static void	draw_line(t_config *conf, t_ray ray, int col)
{
	int		i;
	int		x_texture_position;
	t_image	*tex;

	tex = correct_texture(conf, ray.color);
	i = -1;
	if (ray.color == 1 || ray.color == 2)
		x_texture_position = (int)(floor(ray.end.x + 1)) % TILE_SIZE;
	else
		x_texture_position = (int)(floor(ray.end.y + 1)) % TILE_SIZE;
	while (++i < conf->img_game->h)
	{		
		if (i >= conf->pov->center - (ray.height / 2)
			&& i <= conf->pov->center + (ray.height / 2))
		{
			ray.color = texture_pixel_color(conf,
					p(x_texture_position, (double)i), ray, tex);
			ray.color = shade(ray.dist_wall, ray.color, conf);
			put_pixel_img(conf->img_game, col, i, ray.color);
		}
		else if (i < conf->pov->center)
			put_pixel_img(conf->img_game, col, i, conf->ceiling);
		else
			put_pixel_img(conf->img_game, col, i, conf->floor);
	}
}

static int	set_wall_color(double dist, int angle, t_config *conf)
{
	if (angle < 0)
		angle = r_angle(360) + angle;
	else if (angle >= r_angle(360))
		angle = angle - r_angle(360);
	if (dist == nearest_horizontal(conf, angle))
	{
		if (angle < r_angle(180))
			return (1);
		else
			return (2);
	}
	else
	{
		if (angle > r_angle(90) && angle < r_angle(270))
			return (3);
		else
			return (4);
	}
}

void	raycasting3d(t_config *conf)
{
	t_ray	ray;
	int		column;

	ray.angle = r_angle(30);
	column = 0;
	while (ray.angle > -r_angle(30) && column < WIN_W)
	{
		ray.dist_wall = magic_distance(conf, ray.angle);
		ray.end = add_vec(conf->pov->p,
				vec((ray.angle + conf->pov->angle), ray.dist_wall));
		ray.color = set_wall_color(ray.dist_wall,
				conf->pov->angle + ray.angle, conf);
		ray.height = ray.dist_wall * fcos(abs(ray.angle));
		ray.height = ((double)TILE_SIZE / ray.height) * conf->pov->dtp;
		draw_line(conf, ray, column);
		column++;
		ray.angle--;
	}
}
