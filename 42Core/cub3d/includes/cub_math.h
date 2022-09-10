/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_math.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:14:20 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/19 18:21:02 by dareias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_MATH_H
# define CUB_MATH_H

# include <math.h>

// I HATE THE NORM SO SO SO SO SO SO SO SO MUCH FOR MAKING ME DO THIS ;(
# define WIN_W		1000
# define PI 3.14159265

typedef struct s_point
{
	double	x;
	double	y;
}	t_point;

typedef struct s_vector
{
	int		angle;
	double	mag;
}	t_vector;

double		fcos(int angle);
double		fsin(int angle);
double		ftan(int angle);
t_point		p(double x, double y);
t_point		add_point(t_point p1, t_point p2);
t_vector	vec(int angle, double mag);
t_point		add_vec(t_point p, t_vector v);
t_point		vertical_step(int a);
t_point		horizontal_step(int a);
t_point		map_vertical_step(int a);
t_point		map_horizontal_step(int a);

#endif
