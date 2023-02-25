#ifndef ANIMATION_H
# define ANIMATION_H

# include "utils.h"
# include <mlx.h>
# include <stdlib.h>

enum fig_type {
	CIRCLE,
	RECT
};

typedef struct s_frame {
	int		x;
	int		y;
	int		color;
	int		figure_size;
}		t_frame;

typedef struct s_animation {
	t_list *		frames;
	enum fig_type	fig_type;
	t_frame			current_frame; // The configuration of the current frame
	int				delay; // How many fps it takes to change animation frams
	int				_tmp_delay; // Delay Iterator
	int				current_frame_num; // Which frame is selected
	long int		fps;	// Frames per second, this could be delayed
	long int		last_updated; // When was the last update
	long int		frame_count; // The frame count
}		t_animation;

typedef struct s_animator {
	t_list *	animations;
	t_win *		win;
	t_img *		img;
}	t_animator;

int	update(t_animator *a);
t_animation * ball_animation(int delay, int fps);
t_animation * rect_animation(int delay, int fps);
t_animation * orbit(int delay, int fps);
t_animation * semaphore(int delay, int fps);
int destroy_animator(t_animator * a);
#endif
