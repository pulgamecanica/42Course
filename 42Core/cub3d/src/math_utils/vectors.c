/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- & arosado- <marvin@42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:08:23 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:09:57 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_point	p(double x, double y)
{
	t_point	p;

	p.x = x;
	p.y = y;
	return (p);
}

t_point	add_point(t_point p1, t_point p2)
{
	return (p(p1.x + p2.x, p1.y + p2.y));
}

t_vector	vec(int angle, double mag)
{
	t_vector	vec;

	vec.angle = angle;
	if (vec.angle < 0)
		vec.angle = r_angle(360) + vec.angle;
	else if (vec.angle >= r_angle(360))
		vec.angle = vec.angle - r_angle(360);
	vec.mag = mag;
	return (vec);
}

t_point	add_vec(t_point p, t_vector v)
{
	t_point	res;

	res.x = p.x + (fcos(v.angle) * v.mag);
	res.y = p.y - (fsin(v.angle) * v.mag);
	return (res);
}
