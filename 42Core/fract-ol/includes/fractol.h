/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:23:07 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:23:12 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FRACTOL_H
# define FRACTOL_H

# include <math.h>
# include <mlx.h>
# include "config.h"

/* Colors */
int			get_r(int trgb);
int			get_g(int trgb);
int			get_b(int trgb);
int			get_opacity(int trgb);
int			gen_trgb(int opacity, int red, int green, int blue);

/* MLX Lib Genereal, Image, Window, keys */
int			exit_fract_ol(t_window *win);
int			handle_keys(int key_code, void *p);
int			handle_mouse(int button, int x, int y, void *params);
void		put_pixel_img(t_data *img, int x, int y, int color);
t_data		*new_image(t_window *window, int height, int width);
t_window	*new_window(int height, int width, char *str);

/* Complex numbers, Math */
double		norm(t_complex inum);
t_complex	ipow(t_complex inum);
t_complex	iplus(t_complex inum1, t_complex inum2);
t_complex	get_complex(int x, int y, t_cplane *plane);
t_cplane	*new_cplane(t_data *img, int ac, char **av);

/* Fractals */
void		create_mandelbrot(t_cplane *p);
void		create_julia_set(t_cplane *p);
void		create_burning_ship(t_cplane *p);
void		zoom_in(t_cplane *p);
void		zoom_out(t_cplane *p);
int			key_move_plane(int key_code, t_cplane *p);
#endif
