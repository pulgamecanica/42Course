/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourname- <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 4242/42/42 42:42:42 by you-              #+#    #+#             */
/*   Updated: 4242/42/42 42:42:42 by you-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>

int	gen_trgb(int opacity, int red, int green, int blue)
{
	if (opacity > 255 || red > 255 || green > 255 || blue > 255)
		return (0);
	return (opacity << 24 | red << 16 | green << 8 | blue);
}

int	get_opacity(int trgb)
{
	return ((trgb >> 24) & 0XFF);
}

int	get_r(int trgb)
{
	return ((trgb >> 16) & 0XFF);
}

int	get_g(int trgb)
{
	return ((trgb >> 8) & 0XFF);
}

int	get_b(int trgb)
{
	return (trgb & 0xFF);
}
