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
	destroy_image(ptr);
}

void	destroy_animation(void * ptr) {
	t_animation * a;

	a = (t_animation *)ptr;
	if (!a)
		return ;
	ft_lstclear(&a->frames, _helper_destroy_img);
	free(a->name);
	free(a);
}