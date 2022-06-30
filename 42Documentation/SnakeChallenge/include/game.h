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

#ifndef GAME_H
# define GAME_H

# define MAX_LEN 1000

typedef enum e_bool {
	false,
	true
}	t_bool;

typedef struct s_point
{
	int	x;
	int	y;
}		t_point;

typedef struct s_snake
{
	t_point	coords[MAX_LEN];
	t_point	direction;
	t_bool	can_change;
	int		len;
}		t_snake;

typedef struct s_game
{
	int			bg_color;
	short int	speed;
	short int	tile_size;
	t_snake		snake;
	t_bool		pause;
	t_img		img;
	t_img		food_img;
	t_point		food;

}		t_game;

t_snake	init_snake(void);
int		game_width(t_game *game);
int		game_height(t_game *game);
void	put_snake(t_game *game);
void	remove_snake(t_game *game);
int		update(t_game *game);
void *	random_food(int, void *);

#endif
