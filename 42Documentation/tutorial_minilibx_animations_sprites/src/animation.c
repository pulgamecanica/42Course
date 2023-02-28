#include "libft.h"
#include "utils.h"
#include "game.h"
#include "animation.h"

void	update_animaiton(t_animation * a, t_ent * ent, t_img game_img) {
	t_img *	img;
	int x, y;

	if (!a || !ent)
		return ;
	if (a->_tmp_delay++ == a->delay) {
		a->_tmp_delay = 0;
		a->current_frame_num++;
		a->current_frame_num %= ft_lstsize(a->frames);
	}
	img = (t_img *)ft_lstget(a->frames, a->current_frame_num)->content;
	x = ent->x % img->win.width;
	y = ent->y % img->win.height;
	put_img_to_img(game_img, *img, x, y);
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
}