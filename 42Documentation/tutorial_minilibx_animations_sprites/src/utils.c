#include <sys/time.h>
#include "get_next_line.h"
#include "utils.h"
#include "game.h"

uint64_t	gettimeofday_ms(void) {
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

uint64_t	timestamp_in_ms(t_game * game) {
	if (!game)
		return 0;
	if (game->created_at == 0)
		game->created_at = gettimeofday_ms();
	return (gettimeofday_ms() - game->created_at);
}

int		ft_list_size(char ** list) {
	int i;

	if (!list || !*list)
		return 0;
	i = 0;
	while (list[i])
		i++;
	return i;
}

void	free_list(char ** list) {
	int	i;

	if (!list)
		return ;
	i = 0;
	while(list[i]) {
		free(list[i]);
		i++;
	}
	free(list);
}

int		key_released(int key, void *param) {
	t_game *	game;

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
	t_game *	game;

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

char * get_next_line_nl(int fd) {
	char *	line;
	char *	new_line;

	line = get_next_line(fd);
	if (!line) {
		close(fd);
		return NULL;
	}
	if (line[ft_strlen(line) - 1] != '\n')
		return line;
	if (ft_strlen(line) <= 1) {
		free(line);
		return NULL;
	}
	new_line = (char *)ft_calloc(sizeof(char), ft_strlen(line));
	ft_strlcpy(new_line, line, ft_strlen(line));
	free(line);
	return (new_line);
}
