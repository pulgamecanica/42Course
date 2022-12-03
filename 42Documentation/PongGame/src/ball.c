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
	int color;

	x_v = (rand() % 10) + 5;
	y_v = (rand() % 10) + 5;
	if (x_v % 2)
		x_v = -x_v;
	if (y_v % 2)
		y_v = -y_v;
	color = 0xebe834;
	return ((t_ball){(t_point){x, y}, (t_point){x_v, y_v}, size, color});
}

static t_bool hit_bottom(t_game *game, t_ball *ball)
{
    return (ball->position.y + ball->size >= game->img.h);
}

static t_bool hit_top(t_game *game, t_ball *ball)
{
	(void)game;
	return (ball->position.y < 0);
}

static void move_ball(t_game *game, t_ball *power_ball)
{
	if (hit_bottom(game, power_ball) || hit_top(game, power_ball))
		power_ball->velocity.y = -power_ball->velocity.y;
	power_ball->position.x += power_ball->velocity.x;
	power_ball->position.y += power_ball->velocity.y;
}

void move_balls(t_game *game)
{
	t_list	*tmp;

	move_ball(game, &game->ball);
	tmp = *game->powers;
	while (tmp)
	{
		move_ball(game, tmp->content);
		tmp = tmp->next;
	}
}

void	put_ball(t_game *game)
{
	draw_circle((t_square){
		game->ball.position.x,
		game->ball.position.y,
		game->ball.size,
		game->ball.color
	}, game->img);
	mlx_put_image_to_window (game->img.win.mlx_ptr, game->img.win.win_ptr,
			game->img.img_ptr, 0, 0);
}

static void	bounce(t_ball *ball)
{
	ball->velocity.x = -ball->velocity.x;
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

static void bounce_angle(t_game *game) {
	t_paddle *paddle;
	int paddle_middle;
	int factor;

	paddle = NULL;
	if (check_paddle_bounce_ball(&game->paddle_l, &game->ball, game->tile_size))
		paddle = &game->paddle_l;
	else if (check_paddle_bounce_ball(&game->paddle_r, &game->ball, game->tile_size))
		paddle = &game->paddle_r;
	if (!paddle)
		return;
	paddle_middle = paddle->coords[0].y * game->tile_size + ((paddle->len * game->tile_size) / 2);
	factor = paddle_middle - game->ball.position.y;
	printf("Middle %d\t - Paddle Position: %d\t - Ball Position: %d\n", paddle_middle, paddle->coords[0].y * game->tile_size, game->ball.position.y);
	printf("Factor %d\n", factor);
	if (factor < 0)
		game->ball.velocity.y = -game->ball.velocity.y;
	game->ball.velocity.y += 0.05 * factor;
	if (game->ball.velocity.x > 0)
		game->ball.velocity.x += 2;
	else
		game->ball.velocity.x -= 2;
}

void check_paddles_bounce_balls(t_game *game)
{
	t_list *tmp;
	t_list *tmp_next;
	t_ball *ball;

	if (check_paddle_bounce_ball(&game->paddle_l, &game->ball, game->tile_size) ||
		check_paddle_bounce_ball(&game->paddle_r, &game->ball, game->tile_size)) {
		bounce(&game->ball);
		bounce_angle(game);
	}
	tmp = *game->powers;
	while (tmp)
	{
		tmp_next = tmp->next;
		ball = (t_ball *)tmp->content;
		if (check_paddle_bounce_ball(&game->paddle_l, ball, game->tile_size) ||
		check_paddle_bounce_ball(&game->paddle_r, ball, game->tile_size)) {
			bounce(ball);
		}
		if (ball->position.x < 0)
		{
			update_player_powers(ball, &game->paddle_r);
			ft_lstdelelem(game->powers, tmp, &free);
		}
		else if (ball->position.x > game->img.w)
		{
			update_player_powers(ball, &game->paddle_l);
			ft_lstdelelem(game->powers, tmp, &free);
		}
		tmp = tmp_next;
	}

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