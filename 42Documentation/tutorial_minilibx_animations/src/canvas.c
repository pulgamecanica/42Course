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

void help(t_window *win);

int size = 10;
int follow = 0;

int	continuous_read_keys(int key_pressed, void *param)
{

	t_img *img;

	img = (t_img *)param;
	if (!img)
		exit_tutorial(img);
	else if (key_pressed == UP)
	{
		if (size < img->w / 4)
			size++;
	}
	else if (key_pressed == DOWN)
	{
		if (size > 1)
			size--;
	}
	else if (key_pressed == COLOR)
		get_color(1);
	return (0);
}

int	read_keys(int key_pressed, void *param)
{
	t_img *img;

	img = (t_img *)param;
	if (key_pressed == ENTER)
		draw_square((t_square){0, 0, img->w, 0xFFFFFF}, img);
	else if (key_pressed == ESC || !img)
		exit_tutorial(img);
	else if (key_pressed == HELP)
		help(img->win);
	else if (key_pressed == FOLLOW)
		follow = !follow;
	else if (key_pressed == COLOR)
		get_color(1);
	else if (key_pressed == PENGUIN)
		penguin(img);	
	else
		return (-1);
	mlx_put_image_to_window(img->win->mlx_ptr, img->win->win_ptr, img->img_ptr, 0, 0);
	return (0);
}

int follow_mouse(int mouse_x, int mouse_y, void *param)
{
	int 		x;
	int 		y;
	t_img 		*img;
	t_square	s;

	img = (t_img *) param;
	if (!img)
		return (1);
	if (mouse_x < 0 || mouse_y < 0 || mouse_x > img->w || mouse_y > img->h)
		return (2);
	if (!follow)
		return (3);
	x = ((mouse_x % img->w) / size) * size;
	y = ((mouse_y % img->h) / size) * size;
	s = (t_square){x, y, size, get_color(0)};
	draw_square(s, img);
	mlx_put_image_to_window(img->win->mlx_ptr, img->win->win_ptr, img->img_ptr, 0, 0);
	return (0);
}

int read_mouse(int button, int mouse_x, int mouse_y, void *param)
{
	int 		x;
	int 		y;
	t_img 		*img;
	t_square	s;

	img = (t_img *) param;
	if (!img)
		return (-1);
	if (mouse_x < 0 || mouse_y < 0 || mouse_x > img->w || mouse_y > img->h)
		return (2);
	x = ((mouse_x % img->w) / size) * size;
	y = ((mouse_y % img->h) / size) * size;
	s = (t_square){x, y, size, get_color(0)};
	draw_square(s, img);
	mlx_put_image_to_window(img->win->mlx_ptr, img->win->win_ptr, img->img_ptr, 0, 0);
	(void)button;
	return (0);
}
