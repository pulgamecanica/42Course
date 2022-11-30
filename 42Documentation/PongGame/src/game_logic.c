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

/* YOU CAN WRITE YOUR OWN FUNCTION DOCUMENTATION HERE*/
int	update(t_game *game)
{
	static short int	fps = 0;

	if (game->pause || fps-- != 0)
		return (1);
	fps = game->speed * 10;
	{
		remove_paddles(game);
		remove_ball(game);
		check_paddles_bounce_ball(game); 
		move_ball(game);
		move_paddles(game);
		put_ball(game);
		put_paddles(game);
	}
	return (0);
}

static t_bool check_paddle_bounce_ball(t_paddle *paddle, t_ball *ball, int tile_size)
{
	int i;

	i = 0;
	while (i < paddle->len)
	{
		if (coord_inside_square(
			(t_square){paddle->coords[i].x * tile_size, paddle->coords[i].y * tile_size, tile_size, 0},
			ball->position))
			return (true);
		i++;
	}
	return (false);
}

void check_paddles_bounce_ball(t_game *game)
{
	if (check_paddle_bounce_ball(&game->paddle_l, &game->ball, game->tile_size) ||
		check_paddle_bounce_ball(&game->paddle_r, &game->ball, game->tile_size))
		bounce(&game->ball);
}