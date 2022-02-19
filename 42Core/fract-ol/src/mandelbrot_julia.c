/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:08:59 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:10:34 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "fractol.h"

static int	is_unstable(t_complex inum, t_complex z_num, int iter)
{
	t_complex	temp;
	int			i;

	i = 0;
	temp = inum;
	while (i++ < iter)
	{
		temp = iplus(ipow(temp), z_num);
		if (norm(temp) > 2)
			return (i);
	}
	return (0);
}

static int	is_burning_unstable(t_complex inum, t_complex z_num, int iter)
{
	t_complex	temp;
	int			i;

	i = 0;
	temp = inum;
	while (i++ < iter)
	{
		temp.a = fabs(temp.a);
		temp.b = fabs(temp.b);
		temp = iplus(ipow(temp), z_num);
		if (norm(temp) > 2)
			return (i);
	}
	return (0);
}

void	create_mandelbrot(t_cplane *p)
{
	int		x;
	int		y;
	int		red;
	double	st;

	x = 0;
	while (x < p->data_ptr->w)
	{
		y = 0;
		while (y < p->data_ptr->h)
		{
			st = (double)is_unstable(get_complex(x, y, p),
					get_complex(x, y, p), p->iter);
			if (st > 0)
			{
				red = 255 * (st / p->iter);
				put_pixel_img(p->data_ptr, x, y, gen_trgb(0, red, 40, 40));
			}
			y++;
		}
		x++;
	}
}

void	create_julia_set(t_cplane *p)
{
	int		x;
	int		y;
	int		green;
	double	st;

	x = 0;
	while (x < p->data_ptr->w)
	{
		y = 0;
		while (y < p->data_ptr->h)
		{
			st = (double)is_unstable(get_complex(
						x, y, p), p->z_num, p->iter);
			if (st > 0)
			{
				green = 255 * (st / p->iter);
				put_pixel_img(p->data_ptr, x, y, gen_trgb(0, 40, green, 40));
			}
			y++;
		}
		x++;
	}
}

void	create_burning_ship(t_cplane *p)
{
	int		x;
	int		y;
	int		red;
	double	st;

	x = 0;
	while (x < p->data_ptr->w)
	{
		y = 0;
		while (y < p->data_ptr->h)
		{
			st = (double)is_burning_unstable(get_complex(x, y, p),
					get_complex(x, y, p), p->iter);
			if (st > 0)
			{
				red = 255 * (st / p->iter);
				put_pixel_img(p->data_ptr, x, y, gen_trgb(0, red, 40, 40));
			}
			y++;
		}
		x++;
	}
}
