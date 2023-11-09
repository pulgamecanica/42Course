// #include "utils.h"
#include "sprite.h"

int main(int ac, char *av[]) {
 	t_win		tutorial;
 	int i;

	tutorial = new_window(626, 360, "animations");
	if (!tutorial.win_ptr)
		return 2;
	i = 1;
	while (i < ac) {
		t_sprite *	s;
		s = load_file_sprite(&tutorial, av[i++]);
		if (!s)
			continue ;
		destroy_sprite(*s);
		free(s);
	}
	mlx_loop(tutorial.mlx_ptr);
	destroy_window(tutorial);
	return (0);
}