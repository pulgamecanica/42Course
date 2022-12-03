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
#include <unistd.h>
#include "canvas.h"
#include "game.h"

/**************************************************************************/
// DESCRIPTION :
// First the main initializes the window, main image and game structure.
// The first scope draws the background in the image
// The second scope binds all the hooks fot the game
// update(&game) -> Will be called infinitely while the program is running
// read_keys(&game) -> Function is called on KeyDown
/**************************************************************************/
// CHALLENGE :
// You must create the logic of the game:
//  - the movement
//  - scoring system
//  - growth
//  - loose the game, etc
/**************************************************************************/
// HINTS :
// The movement must be updated on the read_keys function
// When snake is looking up, it cannot change direction to look down
// When it's looking to the left, it cannot change to look at the right
// You can create a function to add two points, and represent the direction
// with a point, like a vector: UP[0, 1] DONW[0, -1] LEFT[-1, 0] RIGHT[1, 0]
// Reember to remove the snake like in the example given
// I suggest you to avoid *alloc functions
/**************************************************************************/

#define WIDTH 1000
#define HEIGHT 600
#define TILE_SIZE 10

static void	*routine(void *arg)
{
	static int sleep_time;
	t_game *game;

	game = (t_game *)arg;
	sleep_time = 1000000;
	sleep_time += rand() % 10000000 + 3000000;
	printf("Waiting %d seconds for next power\n", sleep_time / 1000000);
	while (sleep_time > 0)
	{
		pthread_mutex_lock(game->powers_mutex);
		if (game->over)
			return (arg);
		else if (game->party)
			sleep_time = 20000;
    	pthread_mutex_unlock(game->powers_mutex);
		usleep(1000);
		sleep_time -= 1000;
	}
	pthread_mutex_lock(game->powers_mutex);
	if (!game->over && !game->pause)
		ft_lstadd_front(game->powers, ft_lstnew(init_power(game)));
    pthread_mutex_unlock(game->powers_mutex);
    routine(arg);
    return (arg);
}

static void powers_thread(t_game *game)
{
	int fail;
	
	game->powers_thread = (pthread_t *)ft_calloc(sizeof(pthread_t), 1);
	if (!game->powers_thread)
		return;
	game->powers_mutex = (pthread_mutex_t *)ft_calloc(sizeof(pthread_mutex_t), 1);
	if (!game->powers_mutex)
	{
		free(game->powers_thread);
		return ;
	}

	pthread_mutex_init(game->powers_mutex, NULL);
	fail = pthread_create(game->powers_thread , NULL, &routine, game);
	if (fail)
		return ;
}

int	main(void)
{
	t_win	win;
	t_img	img;
	t_game	game;
	t_list	**root;

	srand(time(NULL));
	root = (t_list **)ft_calloc(sizeof(t_list*), 1);
	win = new_program(WIDTH, HEIGHT, "SnakeGame");
	if (!win.win_ptr)
		return (2);
	img = new_img(WIDTH, HEIGHT, win);
	/* bg_color, score1, score2, speed, tile_size, paddle_l, paddle_r, pause, party, over, img, actions, power_mutex, powern_thread */
	game = (t_game){0x212121, 0, 0, 40, TILE_SIZE, init_paddle(1, 50), init_paddle(WIDTH / TILE_SIZE - 2, 100),
		init_ball(img.w / 2, rand() % img.h, 10), root, false, false, false, img, init_actions(), NULL, NULL};
	{
		draw_square((t_square){0, 0, img.w, game.bg_color}, img);
		mlx_put_image_to_window (img.win.mlx_ptr, img.win.win_ptr,
			img.img_ptr, 0, 0);
	}
	{
		// mlx_key_hook(win.win_ptr, read_keys, &game);
		mlx_hook(win.win_ptr, 2, 1L<<0,  key_pressed, &game);
		mlx_hook(win.win_ptr, 3, 1L<<1,  key_released, &game);
		mlx_loop_hook(win.mlx_ptr, update, &game);
		mlx_hook(win.win_ptr, 17, 0, exit_game, &game);
	}
	powers_thread(&game);
	mlx_loop(win.mlx_ptr);
	ft_lstclear(root, free);
	free(root);
	return (0);
}
