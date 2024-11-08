#include "utils.h"
#include <mlx.h>

t_win new_window(int w, int h)
{
	void	*mlx_ptr;
	char name[] = "Ft_Game";

	mlx_ptr = mlx_init();
	return ((t_win) {mlx_ptr, mlx_new_window(mlx_ptr, w, h, name), w, h});
}
