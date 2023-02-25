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
	if (img)
	{
		if (img->win)
			free(img->win);
		free(img);
	}
	exit(EXIT_SUCCESS);
}

void help(t_window *win)
{
 	mlx_string_put(win->mlx_ptr, win->win_ptr, 20, 20, gen_trgb(0, 250, 190, 124), "Hello Welcome");
    mlx_string_put(win->mlx_ptr, win->win_ptr, 20, 30, gen_trgb(0, 250, 190, 124), "Press ENTER to generate a new canvas");
    mlx_string_put(win->mlx_ptr, win->win_ptr, 20, 40, gen_trgb(0, 250, 190, 124), "Press F for the brush to follow you");
    mlx_string_put(win->mlx_ptr, win->win_ptr, 20, 50, gen_trgb(0, 250, 190, 124), "Press arrow down to make brush smaller");
    mlx_string_put(win->mlx_ptr, win->win_ptr, 20, 60, gen_trgb(0, 250, 190, 124), "Press arrow up to make brush bigger");
    mlx_string_put(win->mlx_ptr, win->win_ptr, 20, 70, gen_trgb(0, 250, 190, 124), "Press c to change color");
    mlx_string_put(win->mlx_ptr, win->win_ptr, 20, 80, gen_trgb(0, 250, 190, 124), "Press p to invoke the penguin :D");
    mlx_string_put(win->mlx_ptr, win->win_ptr, 20, 90, gen_trgb(0, 250, 190, 124), "Press h(help) to show this menu again");
	printf("*****************************************\n");
	printf("*Hello Welcome				*\n");
	printf("*Press ENTER to generate a new canvas	*\n");
	printf("*Press F for the brush to follow you	*\n");
	printf("*Press arrow down to make brush smaller	*\n");
	printf("*Press arrow up to make brush bigger	*\n");
	printf("*Press c to change color		*\n");
	printf("*Press p to invoke the penguin :D	*\n");
	printf("*Press h(help) to show this menu again	*\n");
	printf("*****************************************\n");
}

int	main(void)
{
	t_window	*win;
	t_img		*img;
	t_img		color_palette;


	win = new_window(450, 400, "tutorial");
	if (!win)
		return (-1);
	img = new_image(win, 400, 400);
	if (!img)
		return (-1);
	printf("Created an image [%d x %d]\n", img->w, img->h);
	draw_square((t_square){0, 0, 400, gen_trgb(0, 192, 42, 124)}, img);
	penguin(img);
	mlx_put_image_to_window(win->mlx_ptr, win->win_ptr, img->img_ptr, 0, 0);
	help(win);
    mlx_key_hook (win->win_ptr, read_keys, img);
	mlx_hook(win->win_ptr, 2, 0, continuous_read_keys, img);
    mlx_mouse_hook (win->win_ptr, read_mouse, img);
	mlx_hook(win->win_ptr, 17, 0, exit_tutorial, img);
	mlx_hook(win->win_ptr, 6, 0, follow_mouse, img);
	/*DRAW PALETTE*/
	color_palette = *new_image(win, 50, 400);
	for (int i = 0; i < 8; i++)
		draw_square((t_square){(color_palette.w / 8) * i, 0, (color_palette.w / 8), get_color(1)}, &color_palette);
	get_color(1);
	mlx_put_image_to_window(win->mlx_ptr, win->win_ptr, color_palette.img_ptr, 0, 400);
	mlx_loop(win->mlx_ptr);
	free(win);
	free(img);
	return (0);
}
