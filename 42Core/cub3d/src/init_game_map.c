/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- & arosado- <marvin@42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 08:24:28 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/02 08:24:33 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <get_next_line.h>
#include "cub3d.h"
#include "ft_printf.h"

static int	valid_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_strchr(" 	01NSWE", line[i]))
			return (0);
		i++;
	}
	return (1);
}

static char	*fix_tabs(char *line)
{
	char	*str;
	int		i;
	int		t;
	int		a;

	str = (char *)ft_calloc(sizeof(char), ft_strlen(line)
			+ (ft_strreps(line, '\t') * 3) + 1);
	if (!str)
		return (NULL);
	i = -1;
	t = 0;
	while (line[++i])
	{
		if (line[i] == '\t')
		{
			a = 0;
			while (a < 4)
				str[(i + (3 * t)) + a++] = ' ';
			t++;
		}
		else
			str[i + (3 * t)] = line[i];
	}
	free(line);
	return (str);
}

static void	add_map_row(char *line, t_config *conf)
{
	int		i;
	char	**a;

	if (!line)
		return ;
	a = (char **)calloc(sizeof(char *), ft_arrlen(conf->map) + 2);
	if (!a)
		return ;
	i = 0;
	while (i < ft_arrlen(conf->map))
	{
		a[i] = ft_strdup(conf->map[i]);
		i++;
	}
	a[i] = ft_strdup(line);
	if (conf->map)
		ft_arrfree(conf->map);
	conf->map = a;
}

static int	set_up_map(t_config *conf)
{
	int		i;
	char	**new_map;

	i = -1;
	conf->width = 0;
	if (!conf || !conf->map)
		return (-1);
	while (conf->map[++i])
		if (ft_strlen(conf->map[i]) > (size_t)conf->width)
			conf->width = ft_strlen(conf->map[i]);
	conf->height = i;
	new_map = (char **)ft_calloc(sizeof(char *), conf->height + 1);
	if (!new_map)
		return (-1);
	i = -1;
	while (++i < conf->height)
	{
		new_map[i] = (char *)ft_calloc(sizeof(char), conf->width + 1);
		ft_memset(new_map[i], ' ', conf->width);
		ft_memcpy(new_map[i], conf->map[i], ft_strlen(conf->map[i]));
	}
	ft_arrfree(conf->map);
	conf->map = new_map;
	return (0);
}

int	init_map(int fd, t_config *conf)
{
	int		fail;
	char	*line;

	fail = 0;
	while (fail != -1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		line = get_next_line_no_nl(line);
		if (ft_strlen(line))
		{
			line = fix_tabs(line);
			if (!valid_line(line))
				fail = -1;
			else
				add_map_row(line, conf);
		}
		free(line);
	}
	if (fail == -1 || set_up_map(conf) == -1)
		return (fail);
	return (check_map_format(conf));
}
