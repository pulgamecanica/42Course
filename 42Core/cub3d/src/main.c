/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 08:24:28 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/20 10:03:12 by dareias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "ft_printf.h"
#include "cub3d.h"

/*
static void	menu(void)
{
	ft_printf("Cub3D\n");
}
*/

void	print_config(t_config *conf)
{
	int	i;

	i = 0;
	printf("Floor Color [%d] - (%d, %d, %d)\n", conf->floor,
		get_r(conf->floor), get_g(conf->floor), get_b(conf->floor));
	printf("Ceiling Color [%d] - (%d, %d, %d)\n", conf->ceiling,
		get_r(conf->ceiling), get_g(conf->ceiling), get_b(conf->ceiling));
	printf("NO [%s]\n", conf->walls.no_path);
	printf("SU [%s]\n", conf->walls.so_path);
	printf("WE [%s]\n", conf->walls.we_path);
	printf("EA [%s]\n", conf->walls.ea_path);
	printf("MAP:\n");
	if (conf->map)
	{
		while (conf->map[i])
			printf("%s\n", conf->map[i++]);
	}
	printf("Map Dimension: [%d x %d]\n", conf->pov->width,
		conf->pov->height);
	printf("Player Position(%f, %f) %dº\n", conf->pov->p.x,
		conf->pov->p.y, conf->pov->angle);
	printf("Distance of the Player to the planeº : %d\n", conf->pov->dtp);
}

void	free_conf(t_config *conf)
{
	if (!conf)
		return ;
	if (conf->walls.no_path)
		free(conf->walls.no_path);
	if (conf->walls.so_path)
		free(conf->walls.so_path);
	if (conf->walls.ea_path)
		free(conf->walls.ea_path);
	if (conf->walls.we_path)
		free(conf->walls.we_path);
	if (conf->map)
		ft_arrfree(conf->map);
	free(conf);
}

//mlx_destroy_window(win->mlx_ptr, win->win_ptr);
int	exit_game(t_config *conf)
{
	if (conf)
		free_conf(conf);
	exit(EXIT_SUCCESS);
}

static void	init_game_window(t_config *conf, t_window *win)
{
	int			map_w;
	int			map_h;

	if (conf->width * MAP_TILE_SIZE < MINIMAP_W)
		map_w = conf->width * MAP_TILE_SIZE;
	else
		map_w = MINIMAP_W;
	if (conf->height * MAP_TILE_SIZE < MINIMAP_H)
		map_h = conf->height * MAP_TILE_SIZE;
	else
		map_h = MINIMAP_H;
	conf->img_map = new_image(win, map_h, map_w);
	conf->img_game = new_image(win, WIN_H, WIN_W);
	paint_map(conf);
	init_walls(conf, win);
	raycasting3d(conf);
	mlx_put_image_to_window(win->mlx_ptr, win->win_ptr,
		conf->img_game->img_ptr, 0, 0);
	mlx_put_image_to_window(win->mlx_ptr, win->win_ptr,
		conf->img_map->img_ptr, 0, 0);
}

int	main(int ac, char **av)
{
	t_window	*win;
	t_config	*conf;

	if (ac != 2)
	{
		ft_printf("Usage: ./cub3d [*.cub file]\n");
		return (-1);
	}
	conf = init_game(av[1]);
	if (!conf)
	{
		ft_printf("ERROR <Map Format>\n");
		return (-1);
	}
	print_config(conf);
	win = new_window(WIN_H, WIN_W, "cub3D");
/*
	mlx_hook(win->win_ptr, 2, 0, handle_keys, conf);
        mlx_hook(win->win_ptr, 3, 0, handle_keys, conf);
*/
        mlx_hook(win->win_ptr, 3, 1L<<1, handle_key_up, conf);
	mlx_hook(win->win_ptr, 2, 1L<<0, handle_key_down, conf);

	mlx_hook(win->win_ptr, 17, 0, exit_game, NULL);
	mlx_loop_hook(win->mlx_ptr, update, conf);
	init_game_window(conf, win);
	mlx_loop(win->mlx_ptr);
	destroy_walls(win, conf->walls);
	free_conf(conf);
	return (0);
}
