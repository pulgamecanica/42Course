#include "game.h"

t_ent * new_ent(t_list * a, int x, int y, int width, int height, enum entity e) {
	t_ent * ent;

	ent = (t_ent *)ft_calloc(sizeof(t_ent), 1);
	if (!ent)
		return NULL;
	*ent = (t_ent){a, 0, x, y, width, height, e};
	return ent;
}

void free_ent(void * ptr) {
	t_ent * ent;

	ent = (t_ent *)ptr;
	if (!ent)
		return ;
	ft_lstiter(ent->animations, destroy_animation);
}

void	destroy_game(t_game * game) {
	if (!game)
		exit(EXIT_FAILURE);
	ft_lstiter(game->ents, free_ent);
	ft_lstclear(&game->ents, free);
}