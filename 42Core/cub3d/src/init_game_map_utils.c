/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game_map_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- & arosado- <marvin@42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 08:24:28 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/02 08:24:33 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

static int	count_players(t_config *conf)
{
	int	p;
	int	i;

	i = -1;
	p = 0;
	while (conf->map[++i])
		p += ft_strreps(conf->map[i], 'N') + ft_strreps(conf->map[i], 'S')
			+ ft_strreps(conf->map[i], 'E') + ft_strreps(conf->map[i], 'W');
	return (p);
}

int	check_map_format(t_config *conf)
{
	int	i;
	int	j;

	if (!conf)
		return (-1);
	if (count_players(conf) != 1)
		return (-1);
	i = -1;
	while (conf->map[++i])
	{
		j = -1;
		while (conf->map[i][++j])
		{
			if (ft_strchr("0NSWE", conf->map[i][j]))
			{
				if (i == 0 || i == conf->height - 1 || j == 0
					|| j == conf->width - 1)
					return (-1);
				if (conf->map[i - 1][j] == ' ' || conf->map[i + 1][j] == ' '
					|| conf->map[i][j - 1] == ' ' || conf->map[i][j + 1] == ' ')
					return (-1);
			}
		}
	}
	return (1);
}
