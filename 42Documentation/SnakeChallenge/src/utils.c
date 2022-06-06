/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SnakeGame.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourname- <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 4242/42/42 42:42:42 by somename-         #+#    #+#             */
/*   Updated: 4242/42/42 42:42:42 by somename-        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "canvas.h"
#include "game.h"

/*
draw_square - Draw each pixel of a square on the image
  Parameters:
    [square] Square that will be drawn
    [img]    Image where square will be drawn
*/
void	draw_square(t_square square, t_img img)
{
	unsigned short int	i;
	unsigned short int	j;

	i = 0;
	while (i < square.size && i + square.y < img.h)
	{
		j = 0;
		while (j < square.size && j + square.x < img.w)
		{
			put_pixel_img(img, j + square.x, i + square.y, square.color);
			j++;
		}
		i++;
	}
}

/*
exit_game - Exit the game, destroy image and window
  Parameter:
    [img] Image instance
*/
int	exit_game(t_img *img)
{
	if (img->img_ptr)
		mlx_destroy_image(img->win.mlx_ptr, img->img_ptr);
	if (img->win.win_ptr)
		mlx_destroy_window (img->win.mlx_ptr, img->win.win_ptr);
	exit(EXIT_SUCCESS);
}
