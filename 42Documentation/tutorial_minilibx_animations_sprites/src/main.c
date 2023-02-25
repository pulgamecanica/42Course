#include "utils.h"
#include "sprite.h"
#include "libft.h"
#include "game.h"

#define ENT_SPEED 2

void	update_animaiton(t_animation * a, t_ent * ent, t_img game_img);

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

int	update(t_game * g) {
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

int ft_exit(t_game * g) {
	if (!g)
		exit(EXIT_FAILURE);
	destroy_game(g);
	destroy_window(g->win);
	exit(EXIT_SUCCESS);
	return 0;
}

int main(void)
{
 	t_win	tutorial;
	
	tutorial = new_window(626, 360, "animations");
	if (!tutorial.win_ptr)
		return (2);
	{
		/* Game */
		/* Game - load animations and ents */

		/* Load link 7 animations */
		t_sprite link_sprite = new_sprite("link", "assets/link.xpm", &tutorial);
		if (!link_sprite.sprite_img.img_ptr) {
			destroy_sprite(link_sprite);
			destroy_window(tutorial);
			return (1);
		}
		sprite_slice link_slice = (sprite_slice){0, 0, 64, 64};
		t_animation * link_run = slice_sprite(link_sprite, link_slice, 0, 7, 0, PLAYER);
		link_slice = (sprite_slice){3*64, 64, 64, 64};
		link_slice = (sprite_slice){4*64, 64, 64, 64};
		t_animation * link_iddle = slice_sprite(link_sprite, link_slice, 0, 1, 0, PLAYER);
		link_slice = (sprite_slice){5*64, 64, 64, 64};
		t_animation * link_atack1 = slice_sprite(link_sprite, link_slice, 0, 5, 10, PLAYER);
		link_slice = (sprite_slice){2*64, 2*64, 64, 64};
		t_animation * link_injured = slice_sprite(link_sprite, link_slice, 0, 6, 8, PLAYER);
		link_slice = (sprite_slice){7*64, 3*64, 64, 64};
		t_animation * link_atack2 = slice_sprite(link_sprite, link_slice, 0, 10, 24, PLAYER);
		link_slice = (sprite_slice){64, 3*64, 64, 64};
		t_animation * link_jump = slice_sprite(link_sprite, link_slice, 0, 6, 15, PLAYER);
		link_slice = (sprite_slice){0, 0, 64, 64};
		t_animation * link_run_mirrored = slice_sprite(link_sprite, link_slice, 1, 7, 0, PLAYER);
		link_slice = (sprite_slice){3*64, 64, 64, 64};
		link_slice = (sprite_slice){4*64, 64, 64, 64};
		t_animation * link_iddle_mirrored = slice_sprite(link_sprite, link_slice, 1, 1, 0, PLAYER);
		link_slice = (sprite_slice){5*64, 64, 64, 64};
		t_animation * link_atack1_mirrored = slice_sprite(link_sprite, link_slice, 1, 5, 10, PLAYER);
		link_slice = (sprite_slice){2*64, 2*64, 64, 64};
		t_animation * link_injured_mirrored = slice_sprite(link_sprite, link_slice, 1, 6, 8, PLAYER);
		link_slice = (sprite_slice){7*64, 3*64, 64, 64};
		t_animation * link_atack2_mirrored = slice_sprite(link_sprite, link_slice, 1, 10, 24, PLAYER);
		link_slice = (sprite_slice){64, 3*64, 64, 64};
		t_animation * link_jump_mirrored = slice_sprite(link_sprite, link_slice, 1, 6, 15, PLAYER);
		ft_lstadd_back(&link_sprite.animations, ft_lstnew(link_iddle));
		ft_lstadd_back(&link_sprite.animations, ft_lstnew(link_run));
		ft_lstadd_back(&link_sprite.animations, ft_lstnew(link_jump));
		ft_lstadd_back(&link_sprite.animations, ft_lstnew(link_atack1));
		ft_lstadd_back(&link_sprite.animations, ft_lstnew(link_injured));
		ft_lstadd_back(&link_sprite.animations, ft_lstnew(link_atack2));
		ft_lstadd_back(&link_sprite.animations, ft_lstnew(link_iddle_mirrored));
		ft_lstadd_back(&link_sprite.animations, ft_lstnew(link_run_mirrored));
		ft_lstadd_back(&link_sprite.animations, ft_lstnew(link_jump_mirrored));
		ft_lstadd_back(&link_sprite.animations, ft_lstnew(link_atack1_mirrored));
		ft_lstadd_back(&link_sprite.animations, ft_lstnew(link_injured_mirrored));
		ft_lstadd_back(&link_sprite.animations, ft_lstnew(link_atack2_mirrored));
		printf("Sprite %s [%d %d], loaded %d animations\n", link_sprite.name, link_sprite.width, link_sprite.height, ft_lstsize(link_sprite.animations));
		/* Load fox 6 animations */
		t_sprite fox_sprite = new_sprite("link", "assets/fox.xpm", &tutorial);
		if (!fox_sprite.sprite_img.img_ptr) {
			destroy_sprite(fox_sprite);
			destroy_window(tutorial);
			return (1);
		}
		sprite_slice fox_slice = (sprite_slice){0, 32, 32, 32};
		t_animation * fox_iddle = slice_sprite(fox_sprite, fox_slice, 0, 14, 10, ANIMAL);
		fox_slice = (sprite_slice){0, 32*2, 32, 32};
		t_animation * fox_run = slice_sprite(fox_sprite, fox_slice, 0, 8, 5, ANIMAL);
		fox_slice = (sprite_slice){0, 32*3, 32, 32};
		t_animation * fox_jump = slice_sprite(fox_sprite, fox_slice, 0, 11, 15, ANIMAL);
		fox_slice = (sprite_slice){0, 32*4, 32, 32};
		t_animation * fox_atack = slice_sprite(fox_sprite, fox_slice, 0, 5, 20, ANIMAL);
		fox_slice = (sprite_slice){0, 32*5, 32, 32};
		t_animation * fox_sleep = slice_sprite(fox_sprite, fox_slice, 0, 6, 25, ANIMAL);
		fox_slice = (sprite_slice){0, 32*6, 32, 32};
		t_animation * fox_lay = slice_sprite(fox_sprite, fox_slice, 0, 7, 30, ANIMAL);
	 	fox_slice = (sprite_slice){0, 32, 32, 32};
		t_animation * fox_iddle_mirrored = slice_sprite(fox_sprite, fox_slice, 1, 14, 10, ANIMAL);
		fox_slice = (sprite_slice){0, 32*2, 32, 32};
		t_animation * fox_run_mirrored = slice_sprite(fox_sprite, fox_slice, 1, 8, 5, ANIMAL);
		fox_slice = (sprite_slice){0, 32*3, 32, 32};
		t_animation * fox_jump_mirrored = slice_sprite(fox_sprite, fox_slice, 1, 11, 15, ANIMAL);
		fox_slice = (sprite_slice){0, 32*4, 32, 32};
		t_animation * fox_atack_mirrored = slice_sprite(fox_sprite, fox_slice, 1, 5, 20, ANIMAL);
		fox_slice = (sprite_slice){0, 32*5, 32, 32};
		t_animation * fox_sleep_mirrored = slice_sprite(fox_sprite, fox_slice, 1, 6, 25, ANIMAL);
		fox_slice = (sprite_slice){0, 32*6, 32, 32};
		t_animation * fox_lay_mirrored = slice_sprite(fox_sprite, fox_slice, 1, 7, 30, ANIMAL);
		ft_lstadd_back(&fox_sprite.animations, ft_lstnew(fox_iddle));
		ft_lstadd_back(&fox_sprite.animations, ft_lstnew(fox_run));
		ft_lstadd_back(&fox_sprite.animations, ft_lstnew(fox_jump));
		ft_lstadd_back(&fox_sprite.animations, ft_lstnew(fox_atack));
		ft_lstadd_back(&fox_sprite.animations, ft_lstnew(fox_sleep));
		ft_lstadd_back(&fox_sprite.animations, ft_lstnew(fox_lay));
		ft_lstadd_back(&fox_sprite.animations, ft_lstnew(fox_iddle_mirrored));
		ft_lstadd_back(&fox_sprite.animations, ft_lstnew(fox_run_mirrored));
		ft_lstadd_back(&fox_sprite.animations, ft_lstnew(fox_jump_mirrored));
		ft_lstadd_back(&fox_sprite.animations, ft_lstnew(fox_atack_mirrored));
		ft_lstadd_back(&fox_sprite.animations, ft_lstnew(fox_sleep_mirrored));
		ft_lstadd_back(&fox_sprite.animations, ft_lstnew(fox_lay_mirrored));
		printf("Sprite %s [%d %d], loaded %d animations\n", fox_sprite.name, fox_sprite.width, fox_sprite.height, ft_lstsize(fox_sprite.animations));
		/* Load red_hood 6 animations */
		//t_sprite red_hood_sprite = new_sprite("link", "assets/red_hood.xpm", &tutorial);
		
		t_ent * ent1 = new_ent(link_sprite.animations, 40, 45, 64, 64, PLAYER);
		t_ent * ent2 = new_ent(fox_sprite.animations, 140, 45, 64, 64, ANIMAL);
		//t_ent * ent3 = new_ent(red_hood_sprite.animations, 0, 40, 45, 64, 64, ENEMY);
		t_actions acts = (t_actions){false, false, false, false, false, false};
		t_game game = (t_game){NULL, acts, gettimeofday_ms(), 0, 0, 64, 0, true, new_file_img("assets/bg.xpm", tutorial), new_img(tutorial.width, tutorial.height, tutorial), tutorial};
		if (!game.bg.img_ptr) {
			destroy_sprite(link_sprite);
			destroy_sprite(fox_sprite);
			destroy_window(tutorial);
			return (1);
		}
		ft_lstadd_back(&game.ents, ft_lstnew(ent1));
		ft_lstadd_back(&game.ents, ft_lstnew(ent2));
		destroy_sprite(link_sprite);
		destroy_sprite(fox_sprite);
		mlx_hook(tutorial.win_ptr, 2, 1L<<0,  key_pressed, &game);
		mlx_hook(tutorial.win_ptr, 3, 1L<<1,  key_released, &game);
		mlx_loop_hook(tutorial.mlx_ptr, update, &game);
		mlx_hook(tutorial.win_ptr, 17, 0, ft_exit, &game);
	}
	mlx_loop(tutorial.mlx_ptr);
	destroy_window(tutorial);
	return (0);
}
