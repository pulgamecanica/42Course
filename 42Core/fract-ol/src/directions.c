/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:08:40 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:08:44 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fractol.h"

static void	key_down(t_cplane *p)
{
	double	percent;

	percent = p->y_gap * (p->data_ptr->h * 0.1);
	p->y_s -= percent;
	p->y_e -= percent;
}

static void	key_up(t_cplane *p)
{
	double	percent;

	percent = p->y_gap * (p->data_ptr->h * 0.1);
	p->y_s += percent;
	p->y_e += percent;
}

static void	key_right(t_cplane *p)
{
	double	percent;

	percent = p->x_gap * (p->data_ptr->w * 0.1);
	p->x_s += percent;
	p->x_e += percent;
}

static void	key_left(t_cplane *p)
{
	double	percent;

	percent = p->x_gap * (p->data_ptr->w * 0.1);
	p->x_s -= percent;
	p->x_e -= percent;
}

int	key_move_plane(int key_code, t_cplane *p)
{
	if (key_code == KEY_UP)
		key_up(p);
	else if (key_code == KEY_DOWN)
		key_down(p);
	else if (key_code == KEY_LEFT)
		key_left(p);
	else if (key_code == KEY_RIGHT)
		key_right(p);
	else if (key_code == KEY_PLUS)
		zoom_in(p);
	else if (key_code == KEY_MINUS)
		zoom_out(p);
	else
		return (0);
	return (1);
}
