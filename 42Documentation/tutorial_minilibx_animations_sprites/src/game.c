#include "game.h"
#include "sprite.h"

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
	ft_lstclear(&ent->animations, free);
}

void	destroy_game(t_game * game) {
	if (!game)
		exit(EXIT_FAILURE);
	ft_lstiter(game->ents, free_ent);
	ft_lstclear(&game->ents, free);
}

void	update_ent(void * ptr1, void * ptr2) {
	t_ent * ent;
	t_game * g;
	t_animation * a;

	ent = (t_ent *)ptr1;
	g = (t_game *)ptr2;
	if (!ent || !g)
		return ;
	a =  (t_animation *)ft_lstget(ent->animations, ent->selected_animation)->content;
	if (!a)
		return ;
	update_animaiton(a, ent, g->game_img);
}

void add_ent_from_file_to_game(t_game * game, char * path) {
	t_sprite *	s;
	t_ent *		ent;
	s = load_file_sprite(&game->win, path);
	if (!s) {
		ft_exit(game);
		return ;
	}
	if (s->animations) {
		ent = new_ent(s->animations, 40, 45, 64, 64, PLAYER);
		ft_lstadd_back(&game->ents, ft_lstnew(ent));
	}
	destroy_sprite(*s);
	free(s);
}

int ft_exit(t_game * g) {
	if (!g)
		exit(EXIT_FAILURE);
	destroy_game(g);
	destroy_window(g->win);
	exit(EXIT_SUCCESS);
	return 0;
}