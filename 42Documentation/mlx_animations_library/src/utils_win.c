#include "utils.h"
#include "libft.h"
#include <stdlib.h>
#include <mlx.h>

t_win *	new_window(int w, int h, char *str) {
	t_win * win;

	win = (t_win *)malloc(sizeof(t_win) * 1);
	if (!win)
		exit(3);
	win->mlx_ptr = mlx_init();
	win->win_ptr = mlx_new_window(win->mlx_ptr, w, h, str);
	win->width = w;
	win->height = h;
	return win;
}

void	destroy_window(t_win * w) {
	if (!w)
		exit(3);
	if (w->mlx_ptr && w->win_ptr) 
		mlx_destroy_window(w->mlx_ptr, w->win_ptr);
	//free(w->mlx_ptr);
	//free(w);
}
