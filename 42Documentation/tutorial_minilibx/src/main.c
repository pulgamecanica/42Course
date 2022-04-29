/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourname- <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 4242/42/42 42:42:42 by you-              #+#    #+#             */
/*   Updated: 4242/01/42 42:42:42 by you-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include <stdio.h>

void	penguin(t_img *img);

void	draw_square(t_square square, t_img *img)
{
	unsigned short int	i;
	unsigned short int	j;

	i = 0;
	while (i < square.size && i < img->h)
	{
		j = 0;
		while (j < square.size && j < img->w)
		{
			put_pixel_img(img, j + square.x, i + square.y, square.color);
			j++;
		}
		i++;
	}
}

int	exit_tutorial(t_img *img)
{
	free(img->win);
	free(img);
	exit(EXIT_SUCCESS);
}

int	main(void)
{
	t_window	*win;
	t_img		*img;
	t_square	square;

	win = new_window(400, 400, "tutorial");
	if (!win)
		return (-1);
	img = new_image(win, 400, 400);
	if (!img)
		return (-1);
	printf("Created an image [%d x %d]\n", img->w, img->h);
	square = (t_square){0, 0, 400, gen_trgb(0, 192, 42, 124)};
	draw_square(square, img);
	penguin(img);
	mlx_put_image_to_window(win->mlx_ptr, win->win_ptr, img->img_ptr, 0, 0);
	mlx_hook(win->win_ptr, 17, 0, exit_tutorial, img);
	mlx_loop(win->mlx_ptr);
	free(win);
	free(img);
	return (0);
}
