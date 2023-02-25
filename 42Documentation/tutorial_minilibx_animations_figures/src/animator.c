#include "utils.h"
#include "animation.h"
#include "libft.h"

static void update_animation(void * ptr) {
	t_animation * a;
	t_list	* elem;
	t_frame * f;

	a = (t_animation *)ptr;
	if (!a)
		return;
	elem = ft_lstget(a->frames, a->current_frame_num);
	f = (t_frame *)elem->content;
	if (!elem || !f)
		return ;
	a->current_frame = *f;
	a->current_frame_num++;
	a->current_frame_num %= ft_lstsize(a->frames);
}

static void	draw_frame(void * ptr1, void * ptr2) {
	t_animation * a;
	t_img  * img;

	a = (t_animation *)ptr1;
	img = (t_img *)ptr2;
	if (!a || !img)
		return ;
	if (a->fig_type == CIRCLE) {
		draw_circle((t_circle){
			a->current_frame.x,
			a->current_frame.y,
			a->current_frame.figure_size,
			a->current_frame.color}, *img);
	} else if (a->fig_type == RECT) {
		draw_rect((t_rect){
			a->current_frame.x,
			a->current_frame.y,
			a->current_frame.figure_size,
			a->current_frame.figure_size,
			a->current_frame.color
		}, *img);
	}
}

int	update(t_animator * animator) {
	if (!animator)
		return (0);
	if (animator->animations)
		ft_lstiter(animator->animations, update_animation);
		
	draw_rect(animator->win->bg, *animator->img);
	if (animator->animations) {
		ft_lstiter_param(animator->animations, draw_frame, animator->img);
	}
	mlx_put_image_to_window(animator->win->mlx_ptr, animator->win->win_ptr, animator->img->img_ptr, 0, 0);
	return (1);
}

static void destroy_animation(void * ptr) {
	t_animation * a;

	a = (t_animation *)ptr;
	if (!a)
		return ;
	ft_lstclear(&a->frames, free);
	ft_putendl_fd("Frames destroyed :)\n", 1);
}

int destroy_animator(t_animator * a) {
	ft_lstiter(a->animations, destroy_animation);
	ft_lstclear(&a->animations, free);
	destroy_image(*a->img);
	destroy_window(*a->win);
	ft_putendl_fd("Animator terminated :)\n", 1);
	exit(0);
}