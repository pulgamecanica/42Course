#include "game.h"

uint64_t	gettimeofday_ms(void) {
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

uint64_t	timestamp_in_ms(t_game * game) {
	if (!game)
		return 0;
	if (game->created_at == 0) {
		game->created_at = gettimeofday_ms();
	}
	return (gettimeofday_ms() - game->created_at);
}

int			key_released(int key, void *param) {
	t_game	*game;

	game = (t_game *)param;
	if (key == UP || key == KEY_W)
		game->actions.key_up = false;
	else if (key == DOWN || key == KEY_S)
		game->actions.key_down = false;
	else if (key == LEFT || key == KEY_A)
		game->actions.key_left = false;
	else if (key == RIGHT || key == KEY_D)
		game->actions.key_right = false;
	else if (key == SPACE)
		game->actions.key_space = false;
	else if (key == KEY_P)
		game->actions.key_p = false;
	else if (key == KEY_TAB) {
		game->selected_ent++;
		game->selected_ent %= ft_lstsize(game->ents);
	}
	else
		return (0);
	return 0;
}
int	key_pressed(int key, void *param) {
	t_game	*game;

	game = (t_game *)param;	
	if (key == UP || key == KEY_W)
		game->actions.key_up = true;
	else if (key == DOWN || key == KEY_S)
		game->actions.key_down = true;
	else if (key == LEFT || key == KEY_A)
		game->actions.key_left = true;
	else if (key == RIGHT || key == KEY_D)
		game->actions.key_right = true;
	else if (key == SPACE)
		game->actions.key_space = true;
	else if (key == KEY_P)
		game->actions.key_p = true;
	else
		return (0);
	return 0;
}
