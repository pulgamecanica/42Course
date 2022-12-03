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

#include <string.h>
#include <stdlib.h>
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

void	draw_circle(t_square square, t_img img)
{
	unsigned short int	i;
	unsigned short int	j;

	i = 0;
	while (i < square.size && i + square.y < img.h)
	{
		j = 0;
		while (j < square.size && j + square.x < img.w)
		{
			if (((j - square.size / 2) * (j - square.size / 2)) + ((i - square.size / 2) * (i - square.size / 2)) < (square.size / 2)*(square.size / 2))
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
int	exit_game(t_game *game)
{
	if (game->img.img_ptr)
		mlx_destroy_image(game->img.win.mlx_ptr, game->img.img_ptr);
	if (game->img.win.win_ptr)
		mlx_destroy_window (game->img.win.mlx_ptr, game->img.win.win_ptr);
	ft_lstclear(game->powers, free);
	free(game->powers);
	game->over = true;
	pthread_join(*game->powers_thread, NULL);
	pthread_mutex_destroy(game->powers_mutex);
	exit(EXIT_SUCCESS);
}

/*
	itoa - itoa function from libft
*/

static int	numlen(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	n = -n;
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int		i;
	int		len;
	char	*str;

	if (n == -2147483648)
		return (strdup("-2147483648"));
	i = 0;
	if (n < 0)
		i = 1;
	len = numlen(n) + i;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	if (n < 0)
		str[0] = '-';
	str[len] = '\0';
	n = n * (((n < 0) * -2) + 1);
	while (len-- != i)
	{
		str[len] = (n % 10) + '0';
		n /= 10;
	}
	return (str);
}






