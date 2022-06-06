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

#include <strings.h>
#include "canvas.h"
#include "game.h"

/*
init_snake - Initialize a new snake with default values
  Default Values:
    [len] 3
    [coords] [{1, 1}, {2, 1}, {3, 1}, {0, 0} ZEROS...]
  Return:
    An instance to the snake created
*/
t_snake	init_snake(void)
{
	t_snake	snake;

	snake.len = 3;
	bzero(snake.coords, MAX_LEN * sizeof(t_point));
	snake.coords[0] = (t_point){1, 1};
	snake.coords[1] = (t_point){2, 1};
	snake.coords[2] = (t_point){3, 1};
	snake.direction = (t_point){1, 0};
	snake.can_change = true;
	printf("Snake Created\n");
	printf("Snake Lenght: %d\n", snake.len);
	printf("   ....     ....\n");
	printf(" ..:  :     :  :.+\n");
	printf("      :.....:\n");
	printf("___________________\n");
	return (snake);
}

/*
put_snake - Represents the snake on the image
  Parameters:
    [game] Instance to the game
  It will use the draw_sqare helper function to
  draw len squares of tile_size on the image at
  the coordenates of each element with the color
  0xaaaaaa
  (You can change this to draw different colors)
*/
void	put_snake(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->snake.len)
	{
		draw_square((t_square){
			game->snake.coords[i].x * game->tile_size,
			game->snake.coords[i].y * game->tile_size,
			game->tile_size - 1,
			0xaaaaaa
		}, game->img);
		mlx_put_image_to_window (game->img.win.mlx_ptr, game->img.win.win_ptr,
			game->img.img_ptr, 0, 0);
		i++;
	}
}

/*
remove_snake - Removes the snake from image
  Parameters:
    [game] Instance to the game
  It will use the draw_sqare helper function to
  draw len squares of tile_size on the image at
  the coordenates of each element with bg_color
  to restore the background.
*/
void	remove_snake(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->snake.len)
	{
		draw_square((t_square){
			game->snake.coords[i].x * game->tile_size,
			game->snake.coords[i].y * game->tile_size,
			game->tile_size - 1,
			game->bg_color
		}, game->img);
		mlx_put_image_to_window (game->img.win.mlx_ptr, game->img.win.win_ptr,
			game->img.img_ptr, 0, 0);
		i++;
	}
}

/*
game_width
  Parameters:
    [game] Instance to the game
  Return:
    return the width of the map
    in relation to the snake tiles
*/
int	game_width(t_game *game)
{
	if (!game)
		return (0);
	return (game->img.w / game->tile_size);
}

/*
game_height
  Parameters:
    [game] Instance to the game
  Return:
    return the height of the map
    in relation to the snake tiles
*/
int	game_height(t_game *game)
{
	if (!game)
		return (0);
	return (game->img.h / game->tile_size);
}
