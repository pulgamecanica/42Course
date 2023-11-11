#include "sprite.h"

/**
 * Util functions to interact with sprites
 * Sprites are a collection of animations
 * Exit Codes
 * 2 -> Memory allocation Error
 **/

t_sprite	new_sprite(char * name, char * file_path, t_win * win) {
	t_img	* img;

	img = new_file_img(file_path, win);
	if (!img)
		exit (2);
	return (t_sprite){NULL, ft_strdup(name),
		ft_strdup(file_path), img, img->w, img->h, 0};
}

static void	add_frame(t_animation * a, t_sprite s, sprite_slice slice, int mirrored) {
	t_img *	frame;
	int			i;
	int			j;

	frame = new_img(slice.width - (slice.padding_x * 2), slice.height - (slice.padding_y * 2), s.sprite_img->win);
	if (!frame)
		exit (2);
	i = 0;
	while(i < slice.height - (slice.padding_y * 2)) {
		j = 0;
		while(j < slice.width - (slice.padding_x * 2)) {
			if (mirrored)
				put_pixel_img(frame, (slice.height - (slice.padding_y * 2)) - j - 1, i, get_pixel_img(s.sprite_img, slice.x + j + slice.padding_x, slice.y + i + slice.padding_y));
			else
				put_pixel_img(frame, j, i, get_pixel_img(s.sprite_img, slice.x + j + slice.padding_x, slice.y + i + slice.padding_y));
			j++;
		}
		i++;
	}
	ft_lstadd_back(&a->frames, ft_lstnew(frame));
}

t_animation *	slice_sprite(t_sprite s, sprite_slice slice, int mirrored, int frames, int delay, char * name) {
	t_animation	*	a;
	int						i;

	a = (t_animation *)calloc(sizeof(t_animation), 1);
	if (!a)
		exit (2);
	*a = (t_animation){NULL, slice.width, slice.height, delay, 0, 0, mirrored, 0, 0, ft_strdup(name)};
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
	ft_lstclear(&s.animations, destroy_animation);
	free(s.animations);
	destroy_image(s.sprite_img);
}