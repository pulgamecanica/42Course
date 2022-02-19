/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zoom.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:09:03 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:10:43 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fractol.h"

void	zoom_in(t_cplane *p)
{
	double	x_percent;
	double	y_percent;

	x_percent = p->x_gap * (p->data_ptr->w * 0.1);
	y_percent = p->y_gap * (p->data_ptr->h * 0.1);
	p->x_s += x_percent;
	p->x_e -= x_percent;
	p->y_s -= y_percent;
	p->y_e += y_percent;
	if (p->x_s < 0 && p->x_e > 0)
		p->x_gap = (fabs(p->x_e) + fabs(p->x_s)) / (p->data_ptr->w);
	else
		p->x_gap = ((p->x_e - p->x_s) / (p->data_ptr->w));
	if (p->y_s > 0 && p->y_e < 0)
		p->y_gap = (fabs(p->y_e) + fabs(p->y_s)) / (p->data_ptr->h);
	else
		p->y_gap = (p->y_s - p->y_e) / (p->data_ptr->h);
}

void	zoom_out(t_cplane *p)
{
	double	x_percent;
	double	y_percent;

	if (p->iter <= 10)
		return ;
	x_percent = p->x_gap * (p->data_ptr->w * 0.1);
	y_percent = p->y_gap * (p->data_ptr->h * 0.1);
	p->x_s -= x_percent;
	p->x_e += x_percent;
	p->y_s += y_percent;
	p->y_e -= y_percent;
	if (p->x_s < 0 && p->x_e > 0)
		p->x_gap = (fabs(p->x_e) + fabs(p->x_s)) / (p->data_ptr->w);
	else
		p->x_gap = ((p->x_e - p->x_s) / (p->data_ptr->w));
	if (p->y_s > 0 && p->y_e < 0)
		p->y_gap = (fabs(p->y_e) + fabs(p->y_s)) / (p->data_ptr->h);
	else
		p->y_gap = (p->y_s - p->y_e) / (p->data_ptr->h);
}
