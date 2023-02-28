#ifndef SPRITE_H
# define SPRITE_H

# include <mlx.h>
# include "utils.h"
# include "libft.h"
# include "animation.h"

typedef struct s_sprite {
	t_list	* animations;
	char	* name;
	char	* file_path;
	t_img	sprite_img;
	int	width;
	int	height;
	int	z_index;
}		t_sprite;

typedef	struct sprite_slice {
	int x;
	int y;
	int width;
	int height;
}	sprite_slice;

typedef struct parsed_slice {
	sprite_slice	slice;
	char *			name;
	unsigned int	frames;
	unsigned int	delay;
}	parsed_slice;

/* Sprite */
t_sprite		new_sprite(char * name, char * file_path, t_win * win);
t_animation *	slice_sprite(t_sprite s, sprite_slice slice, int mirrored, int frames, int delay, enum entity e, char * name);
t_sprite *		load_file_sprite(t_win * win, char * path);
void			destroy_sprite(t_sprite s);

#endif
