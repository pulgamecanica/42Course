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

#ifndef CANVAS_H
# define CANVAS_H

# include <mlx.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>


# ifndef LINUX
#  define ESC 53
#  define ENTER 36
#  define HELP 4
#  define PAUSE 35
#  define UP 126
#  define DOWN 125
#  define KEY_W 13
#  define KEY_S 1
# else
#  define ESC
#  define ENTER
#  define HELP
#  define PAUSE
#  define UP
#  define DOWN
#  define KEY_W
#  define KEY_S
# endif

typedef struct s_win
{
	void	*mlx_ptr;
	void	*win_ptr;
	int		height;
	int		width;
}		t_win;

typedef struct s_img
{
	t_win	win;
	void	*img_ptr;
	char	*addr;
	int		h;
	int		w;
	int		bpp;
	int		endian;
	int		line_len;
}		t_img;

typedef struct s_square {
	unsigned short int	x;
	unsigned short int	y;
	unsigned short int	size;
	int					color;
}		t_square;

/* WINDOW AND IMAGE FUNCTIONS */
t_win	new_program(int w, int h, char *str);
t_img	new_img(int w, int h, t_win window);
void	put_pixel_img(t_img img, int x, int y, int color);
/* HELPER FUNCTIONS */
int		exit_game(t_img *img);
void	draw_square(t_square square, t_img img);
int		key_pressed(int key_pressed, void *param);
int		key_released(int key_pressed, void *param);

#endif
