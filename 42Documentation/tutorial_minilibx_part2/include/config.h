/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourname- <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 4242/42/42 42:42:42 by you-              #+#    #+#             */
/*   Updated: 4242/01/42 42:42:42 by you-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include <mlx.h>
# include <stdlib.h>

# define ESC 53
# define ENTER 36
# define HELP 4
# define FOLLOW 3
# define COLOR 8
# define UP 126
# define DOWN 125
# define PENGUIN 35

typedef struct s_window
{
	void	*mlx_ptr;
	void	*win_ptr;
	int		height;
	int		width;
}		t_window;

typedef struct s_img
{
	t_window	*win;
	void		*img_ptr;
	char		*addr;
	int			h;
	int			w;
	int			bpp;
	int			endian;
	int			line_len;
}		t_img;

/*Window and Images*/
void		put_pixel_img(t_img *img, int x, int y, int color);
t_img		*new_image(t_window *win, int height, int width);
t_window	*new_window(int height, int width, char *str);
/*Colors*/
int 	get_color(int next);
int		gen_trgb(int opacity, int red, int green, int blue);
int		get_opacity(int trgb);
int		get_r(int trgb);
int		get_g(int trgb);
int		get_b(int trgb);
/*Hooks*/
int	exit_tutorial(t_img *img);
int	read_keys(int key_pressed, void *param);
int	continuous_read_keys(int key_pressed, void *param);
int follow_mouse(int mouse_x, int mouse_y, void *param);
int read_mouse(int button, int mouse_x, int mouse_y, void *param);
/*SQUARE*/
typedef struct s_square {
	unsigned short int	x;
	unsigned short int	y;
	unsigned short int	size;
	int					color;
}		t_square;

void	draw_square(t_square square, t_img *img);
void	penguin(t_img *img);

#endif
