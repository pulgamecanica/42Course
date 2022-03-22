/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game_config.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 08:24:28 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/20 10:03:12 by dareias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "get_next_line.h"
#include "ft_printf.h"

static int	get_arr_color(char **arr)
{
	if (ft_arrlen(arr) == 3)
		return (gen_trgb(0, ft_atoi(arr[0]), ft_atoi(arr[1]), ft_atoi(arr[2])));
	return (0);
}

static int	set_config_color(char **elems, t_config *conf)
{
	char	**colors;

	if (ft_strlen(elems[0]) != 1 || (*elems[0] != 'C' && *elems[0] != 'F'))
		return (-1);
	colors = ft_split(elems[1], ',');
	if (ft_arrlen(colors) != 3)
	{
		ft_arrfree(colors);
		return (-1);
	}
	if (ft_atoi(colors[0]) < 0 || ft_atoi(colors[0]) > 255
		|| ft_atoi(colors[1]) < 0 || ft_atoi(colors[1]) > 255
		|| ft_atoi(colors[2]) < 0 || ft_atoi(colors[2]) > 255)
	{
		ft_arrfree(colors);
		return (-1);
	}
	else if (ft_strnstr(elems[0], "F", 1))
		conf->floor = get_arr_color(colors);
	else if (ft_strnstr(elems[0], "C", 1))
		conf->ceiling = get_arr_color(colors);
	ft_arrfree(colors);
	return (1);
}

static int	set_config_texture(char **elems, t_config *conf)
{
	if (ft_strlen(elems[1]) <= 4 || !ft_strnstr(elems[1]
			+ ft_strlen(elems[1]) - 4, ".xpm", 4))
	{
		printf("The File Format [%s] is not supported (.xpm)\n", elems[1]);
		return (-1);
	}
	else if (file_exists(elems[1]) == -1)
		return (-1);
	if (ft_strncmp(elems[0], "NO", ft_strlen(elems[0])) == 0
		&& !conf->walls.no_path)
			conf->walls.no_path = ft_strdup(elems[1]);
	else if (ft_strncmp(elems[0], "SO", ft_strlen(elems[0])) == 0
		&& !conf->walls.so_path)
		conf->walls.so_path = ft_strdup(elems[1]);
	else if (ft_strncmp(elems[0], "WE", ft_strlen(elems[0])) == 0
		&& !conf->walls.we_path)
		conf->walls.we_path = ft_strdup(elems[1]);
	else if (ft_strncmp(elems[0], "EA", ft_strlen(elems[0])) == 0
		&& !conf->walls.ea_path)
		conf->walls.ea_path = ft_strdup(elems[1]);
	else
		return (-1);
	return (1);
}

static	int	set_config_element(char **elems, t_config *conf)
{
	int	fail;

	fail = 1;
	if (ft_arrlen(elems) > 0)
	{
		if (ft_arrlen(elems) != 2)
			fail = -1;
		if (set_config_texture(elems, conf) == -1)
			fail = set_config_color(elems, conf);
	}
	if (elems)
		ft_arrfree(elems);
	return (fail);
}

int	init_elements(int fd, t_config *conf)
{
	int		fail;
	char	*line;
	char	*tmp;

	fail = 0;
	while (fail != -1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		tmp = line;
		line = ft_strndup(tmp, ft_strlen(tmp) - 1);
		free(tmp);
		fail = set_config_element(ft_split(line, ' '), conf);
		free(line);
		if (conf->ceiling != -1 && conf->floor != -1 && conf->walls.no_path
			&& conf->walls.so_path && conf->walls.we_path
			&& conf->walls.ea_path)
			break ;
	}
	if (conf->ceiling == -1 || conf->floor == -1 || !conf->walls.no_path
		|| !conf->walls.so_path || !conf->walls.we_path || !conf->walls.ea_path)
		return (-1);
	return (fail);
}
