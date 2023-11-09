#include "libft.h"
#include "utils.h"
#include "animation.h"

void	update_animation(t_animation * a) {
	if (a) {
		if (a->_tmp_delay++ == a->delay) {
			a->_tmp_delay = 0;
			a->current_frame_num++;
			a->current_frame_num %= ft_lstsize(a->frames);
		}
	}
}

static void	_helper_destroy_img(void * ptr) {
	t_img * img;

	img = (t_img *)ptr;
	if (!img)
		return ;
	destroy_image(*img);
}

void	destroy_animation(void * ptr) {
	t_animation * a;

	a = (t_animation *)ptr;
	if (!a)
		return ;
	ft_lstiter(a->frames, _helper_destroy_img);
	ft_lstclear(&a->frames, free);
	free(a->name);
	free(a);
}