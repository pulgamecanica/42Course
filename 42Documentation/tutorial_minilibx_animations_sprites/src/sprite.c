#include "sprite.h"

t_sprite	new_sprite(char * name, char * file_path, t_win * win) {
	t_img	img;

	img = new_file_img(file_path, *win);
	return (t_sprite){NULL, ft_strdup(name),
		ft_strdup(file_path), img, img.w, img.h, 0};
}

static void	add_frame(t_animation * a, t_sprite s, sprite_slice slice, int mirrored) {
	t_img *	frame;
	int	i;
	int	j;

	frame = (t_img *)calloc(sizeof(t_img), 1);
	if (!frame)
		return ;
	*frame = new_img(slice.width, slice.height, s.sprite_img.win);
	i = 0;
	while(i < slice.width) {
		j = 0;
		while(j < slice.height) {
			if (mirrored)
				put_pixel_img(*frame, slice.height - j - 1, i, get_pixel_img(s.sprite_img, slice.x + j, slice.y + i));
			else
				put_pixel_img(*frame, j, i, get_pixel_img(s.sprite_img, slice.x + j, slice.y + i));
			j++;
		}
		i++;
	}
	ft_lstadd_back(&a->frames, ft_lstnew(frame));
}

t_animation *	slice_sprite(t_sprite s, sprite_slice slice, int mirrored, int frames, int delay, enum entity e, char * name) {
	t_animation *	a;
	int	i;

	a = (t_animation *)calloc(sizeof(t_animation), 1);
	if (!a)
		return NULL;
	*a = (t_animation){NULL, slice.width, slice.height, delay, 0, 0, mirrored, 0, 0, e, ft_strdup(name)};
	i = 0;
	while (i < frames) {
		add_frame(a, s, slice, mirrored);
		slice.x += slice.width;
		if (slice.x >= s.width) {
			slice.x = 0;
			slice.y += slice.height;
		}
		i++;
	}
	return a;
}

void	destroy_sprite(t_sprite s) {
	free(s.name);
	free(s.file_path);
	destroy_image(s.sprite_img);
}