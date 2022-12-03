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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "canvas.h"
#include "game.h"

static void check_ball_status(t_game *game) {
	if (game->ball.position.x < 0)
		game->score2++;
	else if (game->ball.position.x > game->img.w)
		game->score1++;
	else
		return;
	// ft_lstadd_front(game->powers, ft_lstnew(init_power(game)));
	game->ball = init_ball(game->img.w / 2, rand() % game->img.h, game->tile_size);
}

static void show_score(t_game *game) {
	char *score1;
	char *score2;
	int i;

	score1 = ft_itoa(game->score1);
	score2 = ft_itoa(game->score2);

	mlx_string_put(game->img.win.mlx_ptr, game->img.win.win_ptr, game->img.w / 2 - game->img.w * 0.25, game->img.h * 0.1, 0xfffff, score1);
	mlx_string_put(game->img.win.mlx_ptr, game->img.win.win_ptr, game->img.w / 2 + game->img.w * 0.25, game->img.h * 0.1, 0xfffff, score2);
	free(score1);
	free(score2);
	i = 0;
	while (i < game->img.h)
	{
		draw_square((t_square){
				game->img.w / 2 - game->tile_size / 3,
				i,
				game->tile_size / 3,
				0xaaaaaa
			}, game->img);
		i += game->tile_size;
	}
}

/* YOU CAN WRITE YOUR OWN FUNCTION DOCUMENTATION HERE*/
int	update(t_game *game)
{
	static short int	fps = 0;

	if (game->pause || fps-- != 0)
		return (1);
	fps = game->speed * 10;
	{
		// ft_lstadd_front(game->powers, ft_lstnew(init_power(game)));
		remove_paddles(game);
		remove_ball(game);
		remove_powers(game);
		check_paddles_bounce_balls(game); 
		move_balls(game);
		move_paddles(game);
		put_powers(game);
		put_ball(game);
		put_paddles(game);
	}
	show_score(game);
	check_ball_status(game);
	// DELETE POWERS THAT ARE NOT CATCHED
	return (0);
}

