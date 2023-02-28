#include "utils.h"
#include "stdlib.h"
#include <mlx.h>

t_win	new_window(int w, int h, char *str) {
	void *	mlx_ptr;

	mlx_ptr = mlx_init();
	return ((t_win) {mlx_ptr, mlx_new_window(mlx_ptr, w, h, str), w, h});
}

void	destroy_window(t_win w) {
	if (w.mlx_ptr && w.win_ptr)
		mlx_destroy_window(w.mlx_ptr, w.win_ptr);
	if (w.mlx_ptr)
		free(w.mlx_ptr);
}
