/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 08:24:28 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/20 10:03:12 by dareias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "cub3d.h"
#include "libft.h"

static t_config	*init_config(void)
{
	t_config	*conf;

	conf = (t_config *)malloc(sizeof(t_config));
	if (!conf)
		return (NULL);
	conf->floor = -1;
	conf->ceiling = -1;
	conf->walls.no_path = NULL;
	conf->walls.so_path = NULL;
	conf->walls.we_path = NULL;
	conf->walls.ea_path = NULL;
	conf->width = 0;
	conf->height = 0;
	conf->map = NULL;
	conf->pov = NULL;
	return (conf);
}

static void	choose_angle(t_config *conf, char c)
{
	if (c == 'N')
		conf->pov->angle = r_angle(90);
	else if (c == 'S')
		conf->pov->angle = r_angle(270);
	else if (c == 'E')
		conf->pov->angle = r_angle(0);
	else if (c == 'W')
		conf->pov->angle = r_angle(180);
}

static void	set_pov_values(t_config *conf)
{
	conf->pov->width = conf->width * TILE_SIZE;
	conf->pov->height = conf->height * TILE_SIZE;
	conf->pov->mag = MAGNITUDE;
	conf->pov->dtp = (WIN_W / 2) / ftan(r_angle(30));
	conf->pov->center = WIN_H / 2;
}

static int	init_pov(t_config *conf)
{
	int	i;
	int	j;

	conf->pov = (t_pov *)ft_calloc(sizeof(t_pov), 1);
	if (!conf->pov)
		return (-1);
	set_pov_values(conf);
	i = -1;
	while (conf->map[++i])
	{
		j = -1;
		while (conf->map[i][++j])
		{
			if (ft_strchr("NSWE", conf->map[i][j]))
			{
				choose_angle(conf, conf->map[i][j]);
				conf->pov->p.x = (j * TILE_SIZE) + TILE_SIZE / 2;
				conf->pov->p.y = (i * TILE_SIZE) + TILE_SIZE / 2;
			}
		}
	}
	return (1);
}

t_config	*init_game(char	*file_path)
{
	int			fd;
	t_config	*conf;

	if (ft_strlen(file_path) <= 4
		|| !ft_strnstr(file_path + ft_strlen(file_path) - 4, ".cub", 4))
	{
		printf("The File Format [%s] is not supported (.cub)\n", file_path);
		return (NULL);
	}
	fd = open(file_path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	conf = init_config();
	if (!conf)
		return (NULL);
	if (init_elements(fd, conf) == -1 || init_map(fd, conf) == -1
		|| init_pov(conf) == -1)
	{
		close(fd);
		free_conf(conf);
		return (NULL);
	}
	close(fd);
	return (conf);
}
