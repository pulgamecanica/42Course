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

t_bool	coord_inside_square(t_square square, t_point coord)
{
	int min_x = square.x - square.size;
	int max_x = square.x + square.size;
	int min_y = square.y - square.size;
	int max_y = square.y + square.size;
	if (coord.x < min_x || coord.x > max_x)
		return (false);
	if (coord.y < min_y || coord.y > max_y)
		return (false);
	return (true);
}

/*
init_actions
*/
t_actions	init_actions(void)
{
	return (t_actions){false, false, false, false};
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
