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

#include "libft.h"
#include <pthread.h>


# define MAX_LEN 100

typedef enum e_bool {
	false,
	true
}	t_bool;

typedef struct s_point
{
	int	x;
	int	y;
}		t_point;

typedef struct s_paddle
{
	t_point	coords[MAX_LEN];
	t_point direction;
	short int speed;
	int		len;
}		t_paddle;

typedef struct ball
{
	t_point position;
	t_point velocity;
	int		size;
	int		color;
}		t_ball;

typedef struct s_actions
{
	t_bool	paddle_l_up;	// UP
	t_bool	paddle_l_down;	// DOWN
	t_bool	paddle_r_up; 	// KEY_W
	t_bool	paddle_r_down;	// KEY_S
}		t_actions;

typedef struct s_game
{
	int			bg_color;
	int			score1;
	int			score2;
	short int	speed;
	short int	tile_size;
	t_paddle	paddle_l;
	t_paddle	paddle_r;
	t_ball		ball;
	t_list		**powers;
	t_bool		pause;
	t_bool		over;
	t_img		img;
	t_actions	actions;
	pthread_mutex_t *powers_mutex;
	pthread_t *powers_thread;
}		t_game;

t_paddle	init_paddle(int x, short int speed);
t_actions	init_actions(void);
t_ball		init_ball(int x, int y, int size);
t_ball		*init_power(t_game *game);
t_bool		coord_inside_square(t_square square, t_point coord);
char		*ft_itoa(int n);
void		check_paddles_bounce_balls(t_game *game);
void 		update_player_powers(t_ball *ball, t_paddle *paddle);
int			game_width(t_game *game);
int			game_height(t_game *game);
void		put_paddles(t_game *game);
void		put_ball(t_game *game);
void		put_powers(t_game *game);
void		move_paddles(t_game *game);
void		move_balls(t_game *game);
void		remove_paddles(t_game *game);
void		remove_ball(t_game *game);
void		remove_powers(t_game *game);
int			update(t_game *game);
int			exit_game(t_game *game);

#endif
