/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:14:20 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/20 13:46:24 by dareias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define MINIMAP_W	100
# define MINIMAP_H	100
# define WIN_W		1000
# define WIN_H		800
# define TILE_SIZE	64
# define MAP_SCALE	16
// MAP_TILE_SIZE = (int)round((double)TILE_SIZE / (double)MAP_SCALE)
// This must be true ALWAYS!
// Because of the norm ;( we can't have this
//# define MAP_TILE_SIZE        8
# define MAP_TILE_SIZE (int)round((double)TILE_SIZE / (double)MAP_SCALE)
# define MAGNITUDE_ANGLE	3
# define MAGNITUDE		10
# define SPRITE_AMMOUNT 	3
# ifndef LINUX
#  define LINUX
#  define KEY_A	0
#  define KEY_S	1
#  define KEY_D 2
#  define KEY_Q	12
#  define KEY_W	13
#  define KEY_E	14
#  define KEY_UP	126
#  define KEY_DOWN	125
#  define KEY_LEFT	123
#  define KEY_RIGHT 124
#  define KEY_ESC	53
# else
#  define KEY_A 97
#  define KEY_S 115
#  define KEY_D 100
#  define KEY_Q 113
#  define KEY_W 119
#  define KEY_E 101
#  define KEY_UP	65362
#  define KEY_DOWN	65364
#  define KEY_LEFT	65361
#  define KEY_RIGHT	65363
#  define KEY_ESC	65307
# endif

# include <mlx.h>
# include <math.h>
# include "cub_math.h"
# include "config.h"
# include "limits.h"
# include "libft.h"
# include "ft_printf.h"

typedef unsigned int	t_uint;

/* Colors */
int			get_r(int trgb);
int			get_g(int trgb);
int			get_b(int trgb);
int			get_opacity(int trgb);
int			gen_trgb(int opacity, int red, int green, int blue);

/* MLX Lib Genereal, Image, Window, Hooks */
int			exit_game(t_config *conf);
int			handle_keys(int key_code, void *params);
void		free_image(t_image *image);
void		put_pixel_img(t_image *img, int x, int y, int color);
t_image		*new_image(t_window *window, int height, int width);
t_window	*new_window(int height, int width, char *str);

/* MAP */
t_config	*init_game(char *file_path);
int			init_map(int fd, t_config *conf);
int			init_elements(int fd, t_config *conf);
int			check_map_format(t_config *conf);
void		free_conf(t_config *conf);
void		paint_map(t_config *conf);

/* Sprites & Textures */
int			*sort_sprites(t_sprite *sprites, int px, int py);
t_uint		texture_pixel_color(t_config *conf, t_point p, t_ray r, t_image *t);
void		init_walls(t_config *conf, t_window *win);
void		destroy_walls(t_window *win, t_walls walls);

/* Utils */
void		raycasting3d(t_config *conf);
int			r_angle(int angle);
int			file_exists(char *filename);
int			collide(t_config *conf, t_point p, int angle);
double		magic_distance(t_config *conf, int ray_angle);
double		nearest_horizontal(t_config *conf, int a);
double		nearest_vertical(t_config *conf, int a);
double		norm(t_config *conf, int x, int y);
char		*get_next_line_no_nl(char *line);

/* Actions */
void		move(int key_code, t_config *conf);
void		rotate(int key_code, t_config *conf);
int		update(t_config *conf);
int	handle_key_up(int key_code, void *params);
int	handle_key_down(int key_code, void *params);
#endif
