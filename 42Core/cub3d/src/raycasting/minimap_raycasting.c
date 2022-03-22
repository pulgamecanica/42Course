/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_raycasting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 08:24:28 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/17 08:18:49 by dareias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

static int	choose_color(char c)
{
	if (c == '0' || ft_strchr("NSWE", c))
		return (gen_trgb(10, 237, 255, 237));
	else if (c == '1')
		return (gen_trgb(10, 26, 83, 92));
	else
		return (gen_trgb(10, 78, 205, 196));
}

static int	minimap_xs(t_config *conf)
{
	int	xs;

	xs = 0;
	if (conf->pov->width < conf->img_map->w)
		return (0);
	if ((conf->pov->p.x / MAP_SCALE) + (conf->img_map->w / 2)
		< conf->width * MAP_TILE_SIZE)
		xs = (conf->pov->p.x / MAP_SCALE) - (conf->img_map->w / 2);
	else
		xs = (conf->width * MAP_TILE_SIZE) - conf->img_map->w;
	if ((conf->pov->p.x / MAP_SCALE) < (conf->img_map->w / 2))
		xs = 0;
	return (xs);
}

static int	minimap_ys(t_config *conf)
{
	int	ys;

	ys = 0;
	if (conf->pov->height < conf->img_map->h)
		return (0);
	if ((conf->pov->p.y / MAP_SCALE) + (conf->img_map->h / 2)
		< conf->height * MAP_TILE_SIZE)
		ys = (conf->pov->p.y / MAP_SCALE) - (conf->img_map->h / 2);
	else
		ys = (conf->height * MAP_TILE_SIZE) - conf->img_map->h;
	if ((conf->pov->p.y / MAP_SCALE) < (conf->img_map->h / 2))
		ys = 0;
	return (ys);
}

static void	minimap_ray_cast(t_config *conf)
{
	int		m;
	int		a;
	int		xx;
	int		yy;
	t_point	point;

	a = r_angle(30);
	while (a >= -r_angle(30))
	{
		m = magic_distance(conf, a) / MAP_SCALE;
		while (--m > 2)
		{
			point = add_vec(p(conf->pov->p.x / MAP_SCALE,
						conf->pov->p.y / MAP_SCALE),
					vec(conf->pov->angle + a, m));
			xx = point.x - minimap_xs(conf);
			yy = point.y - minimap_ys(conf);
			if (xx > 0 && yy > 0 && xx
				< conf->img_map->w && yy < conf->img_map->h)
				put_pixel_img(conf->img_map, xx, yy, gen_trgb(0, 130, 240, 66));
		}
		a--;
	}
}

void	paint_map(t_config *conf)
{
	int	x;
	int	y;
	int	xs;
	int	ys;
	int	color;

	xs = minimap_xs(conf);
	ys = minimap_ys(conf);
	y = -1;
	while (++y < conf->img_map->h && y / MAP_TILE_SIZE
		< conf->height && y + ys < conf->height * MAP_TILE_SIZE)
	{
		x = -1;
		while (++x < conf->img_map->w && x / MAP_TILE_SIZE
			< conf->width && x + xs < conf->width * MAP_TILE_SIZE)
		{
			color = choose_color(conf->map[(y + ys) / MAP_TILE_SIZE]
				[(x + xs) / MAP_TILE_SIZE]);
			put_pixel_img(conf->img_map, x, y, color);
			if (norm(conf, x + xs, y + ys))
				put_pixel_img(conf->img_map, x, y, gen_trgb(0, 240, 30, 100));
		}
	}
	minimap_ray_cast(conf);
}
