/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_and_mouse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:10:17 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:10:23 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fractol.h"
#include <stdlib.h>
#include <stdio.h>

int	mouse_move_plane(int button, t_complex new_center, t_cplane *p)
{
	double	height;
	double	width;

	if (button == 1)
	{
		p->z_num = new_center;
		return (1);
	}
	height = fabs(p->x_s - p->x_e);
	width = fabs(p->y_s - p->y_e);
	p->x_s = new_center.a - (width / 2);
	p->x_e = new_center.a + (width / 2);
	p->y_s = new_center.b + (height / 2);
	p->y_e = new_center.b - (height / 2);
	if (button == 4)
		zoom_out(p);
	else if (button == 5)
		zoom_in(p);
	return (1);
}

int	handle_keys(int key_code, void *params)
{
	t_cplane	*p;
	t_data		*img;
	int			success;

	p = params;
	success = key_move_plane(key_code, p);
	if (key_code == KEY_ESC)
		exit_fract_ol(p->data_ptr->win);
	else if (!success)
		return (-1);
	img = new_image(p->data_ptr->win, p->data_ptr->h, p->data_ptr->w);
	mlx_destroy_image(p->data_ptr->win->mlx_ptr, p->data_ptr->img_ptr);
	free(p->data_ptr);
	p->data_ptr = img;
	p->fractal_function(p);
	mlx_put_image_to_window(p->data_ptr->win->mlx_ptr,
		p->data_ptr->win->win_ptr, p->data_ptr->img_ptr, 0, 0);
	return (0);
}

int	handle_mouse(int button, int x, int y, void *params)
{
	t_cplane	*p;
	t_data		*img;
	int			success;

	if (button == 4 || button == 5 || button == 1)
	{
		p = params;
		success = mouse_move_plane(button, get_complex(x, y, p), p);
		if (!success)
			return (-1);
		img = new_image(p->data_ptr->win, p->data_ptr->h, p->data_ptr->w);
		mlx_destroy_image(p->data_ptr->win->mlx_ptr, p->data_ptr->img_ptr);
		free(p->data_ptr);
		p->data_ptr = img;
		p->fractal_function(p);
		mlx_put_image_to_window(p->data_ptr->win->mlx_ptr,
			p->data_ptr->win->win_ptr, p->data_ptr->img_ptr, 0, 0);
	}
	return (0);
}
