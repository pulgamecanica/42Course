/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:14:20 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:14:24 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CONFIG_H
# define CONFIG_H

# include <mlx.h>

# ifndef START_ITER
#  define START_ITER 100
# endif
# define KEY_UP 126
# define KEY_DOWN 125
# define KEY_LEFT 123
# define KEY_RIGHT 124
# define KEY_PLUS 30
# define KEY_MINUS 44
# define KEY_ESC 53
# define SCROLL_UP 4
# define SCROLL_DOWN 4

typedef struct s_window
{
	void	*mlx_ptr;
	void	*win_ptr;
	int		height;
	int		width;
}		t_window;

typedef struct s_data
{
	t_window	*win;
	void		*img_ptr;
	char		*addr;
	int			h;
	int			w;
	int			bpp;
	int			endian;
	int			line_len;
}		t_data;

typedef struct s_complex_num
{
	double	a;
	double	b;
}		t_complex;

typedef struct s_complex_plane
{
	t_data		*data_ptr;
	t_complex	z_num;
	void		(*fractal_function)(struct s_complex_plane *);
	int			iter;
	double		x_s;
	double		x_e;
	double		y_s;
	double		y_e;
	double		x_gap;
	double		y_gap;
}		t_cplane;

#endif
