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

#include <time.h>
#include <stdlib.h>
#include "canvas.h"
#include "game.h"

t_ball	init_ball(int x, int y, int size)
{
	int x_v;
	int y_v;
	x_v = (rand() % 10) + 5;
	y_v = (rand() % 10) + 5;
	if (x_v % 2)
		x_v = -x_v;
	if (y_v % 2)
		y_v = -y_v;
	return ((t_ball){(t_point){x, y}, (t_point){x_v, y_v}, size});
}

static t_bool hit_bottom(t_game *game)
{
    return (game->ball.position.y + game->ball.size >= game->img.h);
}

static t_bool hit_top(t_game *game)
{
	return (game->ball.position.y < 0);
}

void	move_ball(t_game *game)
{
	if (hit_bottom(game) || hit_top(game))
		game->ball.velocity.y = -game->ball.velocity.y;

	game->ball.position.x += game->ball.velocity.x;
	game->ball.position.y += game->ball.velocity.y;
}

void	put_ball(t_game *game)
{
	draw_square((t_square){
		game->ball.position.x,
		game->ball.position.y,
		game->ball.size,
		0xebe834
	}, game->img);
	mlx_put_image_to_window (game->img.win.mlx_ptr, game->img.win.win_ptr,
			game->img.img_ptr, 0, 0);
}

void	bounce(t_ball *ball)
{
	ball->velocity.x = -ball->velocity.x;
}

void	remove_ball(t_game *game)
{
	draw_square((t_square){
		game->ball.position.x,
		game->ball.position.y,
		game->ball.size,
		game->bg_color
	}, game->img);
	mlx_put_image_to_window (game->img.win.mlx_ptr, game->img.win.win_ptr,
			game->img.img_ptr, 0, 0);
}