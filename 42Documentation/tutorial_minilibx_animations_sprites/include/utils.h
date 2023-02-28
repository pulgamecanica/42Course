#ifndef UTILS_H
# define UTILS_H

# include <mlx.h>
# include <stdint.h>

typedef struct s_win
{
	void	*mlx_ptr;
	void	*win_ptr;
	int		width;
	int		height;
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

/*Window and Images*/
t_win	new_window(int w, int h, char *str);
t_img	new_file_img(char * path, t_win window);
t_img	new_img(int w, int h, t_win window);
unsigned int	get_pixel_img(t_img img, int x, int y);
void	put_pixel_img(t_img img, int x, int y, int color);
void	put_img_to_img(t_img dst, t_img src, int x, int y);
void	destroy_image(t_img img);
void	destroy_window(t_win w);
void	free_list(char ** list);
int		ft_list_size(char ** list);
char *	get_next_line_nl(int fd);

#endif
