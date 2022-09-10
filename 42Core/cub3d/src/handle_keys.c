/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- & arosado- <marvin@42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 14:08:56 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/29 14:10:30 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
static void	paint_window(t_config *conf)
{
	t_image		*img_map;
	t_image		*img_game;

	img_game = new_image(conf->img_game->win, conf->img_game->h,
			conf->img_game->w);
	mlx_destroy_image(conf->img_game->win->mlx_ptr, conf->img_game->img_ptr);
	free(conf->img_game);
	conf->img_game = img_game;
	raycasting3d(conf);
	mlx_put_image_to_window(conf->img_game->win->mlx_ptr,
		conf->img_game->win->win_ptr, img_game->img_ptr, 0, 0);
	img_map = new_image(conf->img_map->win, conf->img_map->h, conf->img_map->w);
	mlx_destroy_image(conf->img_map->win->mlx_ptr, conf->img_map->img_ptr);
	free(conf->img_map);
	conf->img_map = img_map;
	paint_map(conf);
	mlx_put_image_to_window(conf->img_map->win->mlx_ptr,
		conf->img_map->win->win_ptr, img_map->img_ptr, 0, 0);
}


int	handle_keys(int key_code, void *params)
{
	t_config	*conf;

	conf = params;
	if (key_code == KEY_ESC)
		exit_game(conf);

	if (key_code == KEY_W || key_code == KEY_S || key_code == KEY_D
		|| key_code == KEY_A)
		move(key_code, conf);
	else if (key_code == KEY_LEFT || key_code == KEY_Q || key_code == KEY_RIGHT
		|| key_code == KEY_E || key_code == KEY_DOWN || key_code == KEY_UP)
		rotate(key_code, conf);
	else
		return (0);

	paint_window(conf);
	return (EXIT_SUCCESS);
}
*/
static void	keys_update(int key_code, void *params, int up_down)
{
	t_config	*conf;

	conf = params;
	if ((key_code == KEY_W || key_code == KEY_UP) && conf->pov->n_down != up_down)
		conf->pov->n_down = up_down;
	else if ((key_code == KEY_S || key_code == KEY_DOWN) && conf->pov->s_down != up_down)
		conf->pov->s_down = up_down;
	else if ((key_code == KEY_D || key_code == KEY_RIGHT) && conf->pov->e_down != up_down)
		conf->pov->e_down = up_down;
	else if ((key_code == KEY_A || key_code == KEY_LEFT) && conf->pov->w_down != up_down)
		conf->pov->w_down = up_down;
}

int     handle_key_up(int key_code, void *params)
{
        keys_update(key_code, params, 0);
        return (0);
}

int     handle_key_down(int key_code, void *params)
{
        keys_update(key_code, params, 1);
        return (0);
}
