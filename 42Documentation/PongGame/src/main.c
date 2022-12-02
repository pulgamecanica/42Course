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

int	main(void)
{
	t_win	win;
	t_img	img;
	t_game	game;

	srand(time(NULL));
	win = new_program(800, 500, "SnakeGame");
	if (!win.win_ptr)
		return (2);
	img = new_img(800, 500, win);
	/* bg_color, speed, tile_size, paddle_l, paddle_r, pause, img, actions */
	game = (t_game){0x212121, 50, 10, init_paddle(1, 50), init_paddle(78, 100),
		init_ball(img.w / 2, rand() % img.h, 10), false, img, init_actions()};
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
		mlx_hook(win.win_ptr, 17, 0, exit_game, &game.img);
	}
	mlx_loop(win.mlx_ptr);
	return (0);
}
