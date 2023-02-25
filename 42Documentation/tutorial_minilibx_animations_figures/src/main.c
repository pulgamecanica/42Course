#include "utils.h"
#include "animation.h"
#include "libft.h"

int main(void) {
 	t_win tutorial;
	t_img image;
	t_animator animator3000;

	tutorial = new_window(300, 300, "animations");
	if (!tutorial.win_ptr)
		return (2);
 	image = new_img(300, 300, tutorial);
	animator3000 = (t_animator){NULL, &tutorial, &image};
	ft_lstadd_back(&animator3000.animations, ft_lstnew(ball_animation(5, 32)));
	ft_lstadd_back(&animator3000.animations, ft_lstnew(rect_animation(5, 32)));
	ft_lstadd_back(&animator3000.animations, ft_lstnew(orbit(5, 32)));
	ft_lstadd_back(&animator3000.animations, ft_lstnew(semaphore(5, 32)));
	mlx_hook(tutorial.win_ptr, 17, 0, destroy_animator, &animator3000);
	mlx_loop_hook(tutorial.mlx_ptr, update, &animator3000);
	mlx_loop(tutorial.mlx_ptr);
	return (0);
}