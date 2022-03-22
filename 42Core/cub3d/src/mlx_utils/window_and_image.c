/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_and_image.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- & arosado- <marvin@42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 08:24:28 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/02 08:24:33 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "cub3d.h"

void	free_image(t_image *image)
{
	if (image->img_ptr && image->win)
		mlx_destroy_image(image->win->mlx_ptr, image->img_ptr);
	free(image->addr);
	free(image->img_ptr);
}

void	put_pixel_img(t_image *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
	*(unsigned int *) dst = color;
}

t_image	*new_image(t_window *win, int height, int width)
{
	t_image	*data;

	data = (t_image *)malloc(sizeof(t_image));
	if (!data)
		return (NULL);
	data->win = win;
	data->w = width;
	data->h = height;
	data->img_ptr = mlx_new_image (win->mlx_ptr, width, height);
	data->addr = mlx_get_data_addr(data->img_ptr, &(data->bpp),
			&(data->line_len), &(data->endian));
	return (data);
}

t_window	*new_window(int height, int width, char *str)
{
	t_window	*win;
	void		*mlx_ptr;
	void		*win_ptr;

	mlx_ptr = mlx_init();
	if (!mlx_ptr)
		return (NULL);
	win_ptr = mlx_new_window (mlx_ptr, width, height, str);
	if (!win_ptr)
		return (NULL);
	win = (t_window *)malloc(sizeof(t_window));
	win->mlx_ptr = mlx_ptr;
	win->win_ptr = win_ptr;
	win->height = height;
	win->width = width;
	return (win);
}
