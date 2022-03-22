/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- & arosado- <marvin@42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:08:56 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:10:30 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_image	*new_texture_image(t_window *win, char *filename);

void	init_walls(t_config *conf, t_window *win)
{
	conf->walls.no_textu = new_texture_image(win, conf->walls.no_path);
	conf->walls.so_textu = new_texture_image(win, conf->walls.so_path);
	conf->walls.ea_textu = new_texture_image(win, conf->walls.ea_path);
	conf->walls.we_textu = new_texture_image(win, conf->walls.we_path);
}

static t_image	*new_texture_image(t_window *win, char *filename)
{
	t_image	*data;

	data = (t_image *)malloc(sizeof(t_image));
	if (!data)
		return (NULL);
	data->win = win;
	data->img_ptr = mlx_xpm_file_to_image(win->mlx_ptr, filename,
			&(data->w), &(data->h));
	if (!data->img_ptr)
		ft_printf("Error: Could not load image from xpm file\n");
	data->addr = mlx_get_data_addr(data->img_ptr, &(data->bpp),
			&(data->line_len), &(data->endian));
	return (data);
}

unsigned int	get_texture_color(t_image *texture, int pixel_x, int pixel_y)
{
	return (*(unsigned int *)((texture->addr
			+ (pixel_y * texture->line_len) + (pixel_x * texture->bpp / 8))));
}

unsigned int	texture_pixel_color(t_config *conf, t_point p, t_ray ray,
		t_image *texture)
{
	double	pixel_x;
	double	pixel_y;
	int		top;

	top = (conf->pov->center) - (ray.height / 2);
	pixel_x = p.x / TILE_SIZE * texture->w;
	pixel_y = (p.y - top) / ray.height;
	pixel_y = pixel_y * texture->h;
	return (get_texture_color(texture, pixel_x, pixel_y));
}

void	destroy_walls(t_window *win, t_walls walls)
{
	if (walls.no_textu->img_ptr)
		mlx_destroy_image(win->mlx_ptr, walls.no_textu->img_ptr);
	if (walls.so_textu->img_ptr)
		mlx_destroy_image(win->mlx_ptr, walls.so_textu->img_ptr);
	if (walls.ea_textu->img_ptr)
		mlx_destroy_image(win->mlx_ptr, walls.ea_textu->img_ptr);
	if (walls.we_textu->img_ptr)
		mlx_destroy_image(win->mlx_ptr, walls.we_textu->img_ptr);
}
