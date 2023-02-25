#ifndef UTILS_H
# define UTILS_H

# include <mlx.h>
# include "libft.h"

struct s_img;
struct s_rect;

typedef struct s_rect {
	unsigned short int	x;
	unsigned short int	y;
	unsigned short int	size_w;
	unsigned short int	size_h;
	int					color;
}		t_rect;

typedef struct s_circle {
	unsigned short int	x;
	unsigned short int	y;
	unsigned short int	radius;
	int					color;
}		t_circle;

typedef struct s_win {
	void	*mlx_ptr;
	void	*win_ptr;
	int		height;
	int		width;
	t_rect	bg;
}		t_win;

typedef struct s_img {
	t_win	win;
	void	*img_ptr;
	char	*addr;
	int		h;
	int		w;
	int		bpp;
	int		endian;
	int		line_len;
}		t_img;

/* Window and Images */
t_win	new_window(int w, int h, char *str);
t_img	new_img(int w, int h, t_win window);
void	put_pixel_img(t_img img, int x, int y, int color);
void	destroy_image(t_img img);
void	destroy_window(t_win w);
/* FIGURES */
void	draw_rect(t_rect rect, t_img img);
void	draw_circle(t_circle circle, t_img img);

/* COLORS */
int 	get_color(int next);
int		gen_trgb(int opacity, int red, int green, int blue);
int		get_opacity(int trgb);
int		get_r(int trgb);
int		get_g(int trgb);
int		get_b(int trgb);

#endif

/*Hooks*/
/*
int	exit_tutorial(t_img *img);
int	read_keys(int key_pressed, void *param);
int	continuous_read_keys(int key_pressed, void *param);
int follow_mouse(int mouse_x, int mouse_y, void *param);
int read_mouse(int button, int mouse_x, int mouse_y, void *param);
*/


/*SQUARE*/
/*
typedef struct s_square {
	unsigned short int	x;
	unsigned short int	y;
	unsigned short int	size;
	int					color;
}		t_square;

void	draw_square(t_square square, t_img *img);
void	penguin(t_img *img);

#endif
*/