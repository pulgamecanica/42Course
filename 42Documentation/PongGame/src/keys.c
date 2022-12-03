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

static int set_directions(t_game *game)
{
	if (game->actions.paddle_r_up)
		game->paddle_r.direction = (t_point){0, -1};
	else if (game->actions.paddle_r_down)
		game->paddle_r.direction = (t_point){0, 1};
	else
		game->paddle_r.direction = (t_point){0, 0};
	if (game->actions.paddle_l_up)
		game->paddle_l.direction = (t_point){0, -1};
	else if (game->actions.paddle_l_down)
		game->paddle_l.direction = (t_point){0, 1};
	else
		game->paddle_l.direction = (t_point){0, 0};
	return (1);
}

static int execute_action(int key, t_game *game, t_bool action)
{
	if (key == UP)
		game->actions.paddle_l_up = action;
	else if (key == DOWN)
		game->actions.paddle_l_down = action;
	else if (key == KEY_W)
		game->actions.paddle_r_up = action;
	else if (key == KEY_S)
		game->actions.paddle_r_down = action;
	else
		return (-1);
	set_directions(game);
	return (1);
}

static void pause_panel(t_game *game) {
	if (game->pause)
	{
		mlx_string_put(game->img.win.mlx_ptr, game->img.win.win_ptr, game->img.w / 2 - 15, game->img.h / 2, 0x0fff, "Pause");
	}
}

int	key_released(int key_released, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	return execute_action(key_released, game, false);
}

int	key_pressed(int key_pressed, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (key_pressed == ESC || !game->img.img_ptr)
		exit_game(game);
	else if (key_pressed == PAUSE) {
		game->pause = !game->pause;
		pause_panel(game);
	}
	return execute_action(key_pressed, game, true);
}
