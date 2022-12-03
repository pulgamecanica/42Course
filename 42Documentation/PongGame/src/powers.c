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

#include "libft.h"
#include "canvas.h"
#include "game.h"

t_ball *init_power(t_game *game) {
	t_ball *power;

	power = (t_ball *)ft_calloc(sizeof(t_ball), 1);
	if (!power)
		return (NULL);
	power->position.x = game->img.w / 2;
	power->position.y = game->img.h / 2;
	power->velocity.x = (rand() % 10) + 5;;
	power->velocity.y = (rand() % 10) + 5;
	if (power->velocity.x % 2)
		power->velocity.x *= -1;
	if (power->velocity.y % 2)
		power->velocity.y *= -1;
	power->size = rand() % 5 + 8;
	power->color = rand();
	return (power);
}

void remove_powers(t_game *game)
{
	t_list	*tmp;
	t_ball	*ball;

	tmp = *game->powers;
	while (tmp)
	{
		ball = (t_ball *)tmp->content;
		draw_square((t_square){
			ball->position.x,
			ball->position.y,
			ball->size,
			game->bg_color
		}, game->img);
		tmp = tmp->next;
	}
	mlx_put_image_to_window (game->img.win.mlx_ptr, game->img.win.win_ptr,
			game->img.img_ptr, 0, 0);
}

void put_powers(t_game *game)
{
	t_list	*tmp;
	t_ball	*ball;

	tmp = *game->powers;
	while (tmp)
	{
		ball = (t_ball *)tmp->content;
		draw_square((t_square){
			ball->position.x,
			ball->position.y,
			ball->size,
			ball->color
		}, game->img);
		tmp = tmp->next;
	}
	mlx_put_image_to_window (game->img.win.mlx_ptr, game->img.win.win_ptr,
			game->img.img_ptr, 0, 0);
}

void update_player_powers(t_ball *ball, t_paddle *paddle)
{
	if (ball->size == 8)
	{
		printf("PowerDown! Sloweed Down!\n");
		paddle->speed -= 5;
	}
	else if (ball->size == 9 && paddle->len > 5)
	{
		printf("PowerDown! Paddle Shrinked!\n");
		paddle->len--;
	}
	else if (ball->size == 10)
	{
		printf("PowerUp! Paddle Accelerated!\n");
		paddle->speed += 5;
	}
	else if (ball->size == 11 && paddle->len < 60)
	{
		printf("PowerUp! Paddle grew!\n");
		paddle->len++;
		paddle->coords[paddle->len - 1].x = paddle->coords[0].x;
		paddle->coords[paddle->len - 1].y = paddle->coords[paddle->len - 2].y + 1;
	}
	else if (ball->size == 12 && paddle->len < 60)
	{
		printf("PowerUp! Paddle Accelerated & Grew a lot!\n");
		paddle->speed += 8;
		paddle->len += 3;
		paddle->coords[paddle->len - 1].x = paddle->coords[0].x;
		paddle->coords[paddle->len - 1].y = paddle->coords[paddle->len - 4].y + 1;
		paddle->coords[paddle->len - 2].x = paddle->coords[0].x;
		paddle->coords[paddle->len - 2].y = paddle->coords[paddle->len - 4].y + 1;
		paddle->coords[paddle->len - 3].x = paddle->coords[0].x;
		paddle->coords[paddle->len - 3].y = paddle->coords[paddle->len - 4].y + 1;
	}
		
}
