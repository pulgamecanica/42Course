#include "utils.h"
#include "sprite.h"
#include "libft.h"
#include "game.h"

# define ENT_SPEED 2

static int	update(t_game * g) {
	if (!g)
		return 1; 
	if (timestamp_in_ms(g) - g->updated_at < (uint64_t)(1000 / g->fps))
		return 0;
	g->updated_at = timestamp_in_ms(g);
	if (!g->ents)
		return 1;
	t_ent * selected_ent = (t_ent *)ft_lstget(g->ents, g->selected_ent)->content;
	if (selected_ent) {
		if (g->actions.key_left) {
			g->looking_east = false;
			selected_ent->x -= ENT_SPEED;
			selected_ent->selected_animation = 7;
		}
		if (g->actions.key_right) {
			g->looking_east = true;
			selected_ent->x += ENT_SPEED;
			selected_ent->selected_animation = 1;
		}
		if (g->actions.key_up) {
			selected_ent->y -= ENT_SPEED / 2;
			if (g->looking_east) {
				selected_ent->selected_animation = 2;
			} else {
				selected_ent->selected_animation = 8;
			}
		}
		if (g->actions.key_down) {
			selected_ent->y += ENT_SPEED / 2;
			if (g->looking_east) {
				selected_ent->selected_animation = 4;
			} else {
				selected_ent->selected_animation = 10;
			}
		}
		if (g->actions.key_p) {
			if (g->looking_east) {
				selected_ent->selected_animation = 3;
			} else {
				selected_ent->selected_animation = 9;
			}
		}
		if (g->actions.key_space) {
			if (g->looking_east) {
				selected_ent->selected_animation = 5;
			} else {
				selected_ent->selected_animation = 11;
			}
		}
		if (!g->actions.key_right && !g->actions.key_left && !g->actions.key_up && !g->actions.key_down && !g->actions.key_p && !g->actions.key_space) {
			if (g->looking_east)
				selected_ent->selected_animation = 0;
			else
				selected_ent->selected_animation = 6;
		}
	}
	put_img_to_img(g->game_img, g->bg, 0, 0);
	ft_lstiter_param(g->ents, update_ent, g);
	mlx_put_image_to_window(g->game_img.win.mlx_ptr, g->game_img.win.win_ptr, g->game_img.img_ptr, 0, 0);
	return 0;
}

int main(int ac, char *av[]) {
 	t_win		tutorial;
 	t_actions	acts;
 	t_game		game;
 	int i;

	tutorial = new_window(626, 360, "animations");
	if (!tutorial.win_ptr)
		return 2;
	acts = (t_actions){false, false, false, false, false, false};
	game = (t_game){NULL, acts, gettimeofday_ms(), 0, 0, 64, 0, true, new_file_img("assets/bg.xpm", tutorial), new_img(tutorial.width, tutorial.height, tutorial), tutorial};
	if (!game.bg.img_ptr)
		ft_exit(&game);
	if (ac <= 1)
		add_ent_from_file_to_game(&game, "red_hood_linux.animation");
	i = 1;
	while (i < ac)
		add_ent_from_file_to_game(&game, av[i++]);
	mlx_hook(tutorial.win_ptr, 2, 1L<<0,  key_pressed, &game);
	mlx_hook(tutorial.win_ptr, 3, 1L<<1,  key_released, &game);
	mlx_loop_hook(tutorial.mlx_ptr, update, &game);
	mlx_hook(tutorial.win_ptr, 17, 0, ft_exit, &game);
	mlx_loop(tutorial.mlx_ptr);
	return (0);
}