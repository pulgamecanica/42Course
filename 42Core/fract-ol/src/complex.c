/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:08:30 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:08:36 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "fractol.h"
#include "libft.h"

t_cplane	*new_cplane(t_data *img, int ac, char **av)
{
	t_cplane	*plane;

	plane = (t_cplane *)malloc(sizeof(t_cplane));
	if (!plane)
		return (NULL);
	plane->x_s = -2.0f;
	plane->x_e = 2.0f;
	plane->y_s = 2.0f;
	plane->y_e = -2.0f;
	plane->x_gap = (fabs(plane->x_e) + fabs(plane->x_s)) / (img->w);
	plane->y_gap = (fabs(plane->y_e) + fabs(plane->y_s)) / (img->h);
	plane->data_ptr = img;
	if (ac >= 3)
		plane->iter = ft_atoi(av[2]);
	else
		plane->iter = 100;
	plane->z_num.a = 0.285f;
	plane->z_num.b = 0.01f;
	return (plane);
}

t_complex	get_complex(int x, int y, t_cplane *plane)
{
	t_complex	inum;

	inum.a = (x * plane->x_gap) + plane->x_s;
	inum.b = plane->y_s - (y * plane->y_gap);
	return (inum);
}

t_complex	ipow(t_complex inum)
{
	t_complex	res;

	res.a = pow(inum.a, 2) - pow(inum.b, 2);
	res.b = (2 * inum.a * inum.b);
	return (res);
}

t_complex	iplus(t_complex inum1, t_complex inum2)
{
	t_complex	inum;

	inum.a = inum1.a + inum2.a;
	inum.b = inum1.b + inum2.b;
	return (inum);
}

double	norm(t_complex inum)
{
	return (sqrt(pow(inum.a, 2) + pow(inum.b, 2)));
}
