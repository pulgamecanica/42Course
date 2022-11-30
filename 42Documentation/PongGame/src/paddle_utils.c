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
init_paddle - Initialize a new paddle with default values
	Parameters:
		[x] The coordinate x of the paddle
  Default Values:
    [len] 3
    [coords] [{1, 1}, {2, 1}, {3, 1}, {0, 0} ZEROS...]
  Return:
    An instance to the paddle created
*/
t_paddle	init_paddle(int x, short int speed)
{
	t_paddle	paddle;
	int				i;

	paddle.len = 10;
	bzero(paddle.coords, MAX_LEN * sizeof(t_point));
	i = 0;
	while (i < paddle.len)
	{
		paddle.coords[i] = (t_point){x, i + 10};
		i++;
	}
	paddle.direction = (t_point){0, 0};
	paddle.speed = speed;
	printf("Paddle Created\n");
	printf("Paddle Lenght: %d\n", paddle.len);
	return (paddle);
}

/*
move_paddles - Move the paddle according to actions
	Parameters:
		[game] Instance to the game
	Read the game.actions and move the paddles accordingly
*/
static t_bool check_limits(t_game *game, t_point p)
{
	if (p.x < 0 || p.y < 0)
		return (false);
	if (p.x >= game->img.w / game->tile_size || p.y >= game->img.h / game->tile_size)
		return (false);
	return (true);
}

static void move_paddle(t_game *game, t_paddle *paddle) {
	t_point	new_head;

	if (paddle->direction.y == -1) 
	{
		new_head = (t_point){paddle->coords[0].x + paddle->direction.x,
			paddle->coords[0].y + paddle->direction.y};
		if (!check_limits(game, new_head))
			return;
		memcpy(paddle->coords + 1, paddle->coords,
			(paddle->len - 1) * sizeof(t_point));
		memcpy(paddle->coords, &new_head, sizeof(t_point));
	}
	else if (paddle->direction.y == 1)
	{
		new_head = (t_point){paddle->coords[paddle->len - 1].x + paddle->direction.x,
			paddle->coords[paddle->len - 1].y + paddle->direction.y};
		if (!check_limits(game, new_head))
			return;
		memcpy(paddle->coords, paddle->coords + 1,
			(paddle->len - 1) * sizeof(t_point));
		memcpy(paddle->coords + paddle->len - 1, &new_head, sizeof(t_point));
	} 
}

void move_paddles(t_game *game)
{
	move_paddle(game, &game->paddle_l);
	move_paddle(game, &game->paddle_r);
}

/*
put_paddles - Represents the paddle on the image
  Parameters:
    [game] Instance to the game
  It will use the draw_sqare helper function to
  draw len squares of tile_size on the image at
  the coordenates of each element with the color
  0xaaaaaa
  (You can change this to draw different colors)
*/
void	put_paddles(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->paddle_l.len || i < game->paddle_r.len)
	{
		if (i < game->paddle_l.len)
		{
			draw_square((t_square){
				game->paddle_l.coords[i].x * game->tile_size,
				game->paddle_l.coords[i].y * game->tile_size,
				game->tile_size,
				0xaaaaaa
			}, game->img);
		}
		if (i < game->paddle_r.len)
		{
			draw_square((t_square){
			game->paddle_r.coords[i].x * game->tile_size,
			game->paddle_r.coords[i].y * game->tile_size,
			game->tile_size,
			0xaaaaaa
		}, game->img);
		}
		i++;
	}
	mlx_put_image_to_window (game->img.win.mlx_ptr, game->img.win.win_ptr,
			game->img.img_ptr, 0, 0);
}

/*
remove_paddles - Removes the paddle from image
  Parameters:
    [game] Instance to the game
  It will use the draw_sqare helper function to
  draw len squares of tile_size on the image at
  the coordenates of each element with bg_color
  to restore the background.
*/
void	remove_paddles(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->paddle_l.len || i < game->paddle_r.len)
	{
		if (i < game->paddle_l.len)
		{
			draw_square((t_square){
				game->paddle_l.coords[i].x * game->tile_size,
				game->paddle_l.coords[i].y * game->tile_size,
				game->tile_size,
				game->bg_color
			}, game->img);
		}
		if (i < game->paddle_r.len)
		{
			draw_square((t_square){
			game->paddle_r.coords[i].x * game->tile_size,
			game->paddle_r.coords[i].y * game->tile_size,
			game->tile_size,
			game->bg_color
		}, game->img);
		}
		i++;
	}
	mlx_put_image_to_window (game->img.win.mlx_ptr, game->img.win.win_ptr,
			game->img.img_ptr, 0, 0);
}

/*
game_width
  Parameters:
    [game] Instance to the game
  Return:
    return the width of the map
    in relation to the paddle tiles
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
    in relation to the paddle tiles
*/
int	game_height(t_game *game)
{
	if (!game)
		return (0);
	return (game->img.h / game->tile_size);
}
