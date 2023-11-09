#ifndef ANIMATION_H
# define ANIMATION_H

# include "libft.h"

typedef struct s_animation {
	t_list * frames;
	int      width;
	int      height;
	int      delay;              // How many fps it takes to change animation
	int      _tmp_delay;         // Delay Iterator
	int      current_frame_num;  // Which frame is selected
	int      mirrored;
	long int updated_at;         // When was the last update
	long int frame_count;        // The frame count
	char *   name;
} t_animation;

void	update_animation(t_animation * a);
void	destroy_animation(void * ptr);
#endif
