#ifndef GAME_H
# define GAME_H

# ifndef LINUX
#  define ESC 53
#  define ENTER 36
#  define SPACE 0
#  define UP 126
#  define DOWN 125
#  define LEFT 125
#  define RIGHT 125
#  define KEY_W 0
#  define KEY_S 0
#  define KEY_A 115
#  define KEY_D 0
#  define KEY_P 35
#  define KEY_TAB 0
# else
#  define ESC 65307
#  define ENTER 65293
#  define SPACE 32
#  define UP 65362
#  define DOWN 65364
#  define LEFT 65361
#  define RIGHT 65363
#  define KEY_W 119
#  define KEY_S 115
#  define KEY_A 97
#  define KEY_D 100
#  define KEY_P 112
#  define KEY_TAB 65289
# endif

# include <mlx.h>
# include <sys/time.h>
# include <stdint.h>
# include "libft.h"
# include "utils.h"
# include "animation.h"

typedef enum e_bool {
	false,
	true
}	t_bool;

typedef struct s_ent {
	t_list * animations;
	int selected_animation;
	int	x;
	int y;
	int width;
	int height;
	enum entity entity;
}	t_ent;

typedef struct s_actions {
	t_bool	key_up;	// UP Arrow or W
	t_bool	key_down;	// DOWN Arroy or S
	t_bool	key_left; 	// KEY_W Arrow or A
	t_bool	key_right;	// KEY_S Arrow or d
	t_bool	key_space;	// KEY_SPACE
	t_bool	key_p;	// KEY_P
}		t_actions;

typedef struct s_game {
	t_list *	ents;
	t_actions	actions;
	uint64_t	created_at;
	uint64_t	updated_at;
	long int	frame_count;
	int	fps;
	int	selected_ent;
	t_bool looking_east;
	t_img	bg;
	t_img	game_img;
	t_win	win;
}		t_game;

/* Game */
t_ent * new_ent(t_list * a, int x, int y, int width, int height, enum entity e);
void	destroy_game(t_game * game);
/* Utils */
uint64_t	gettimeofday_ms(void);
uint64_t	timestamp_in_ms(t_game * game);
int			key_released(int key_released, void *param);
int			key_pressed(int key_released, void *param);
void		update_ent(void * ptr1, void * ptr2);
void		add_ent_from_file_to_game(t_game * game, char * path);
void		update_animaiton(t_animation * a, t_ent * ent, t_img game_img);
int			ft_exit(t_game * g);

#endif
