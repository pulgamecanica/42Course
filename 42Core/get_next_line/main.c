/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 15:12:53 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/29 15:12:55 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

int	main(int ac, char **av)
{
	int		fd;
	char	*line;

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			ft_putstr("Couldn't read file: ");
			ft_putstr(av[1]);
			ft_putstr("\n");
			return (-1);
		}
		line = get_next_line(fd);
		while (line != NULL)
		{
			ft_putstr(">>>>>");
			ft_putstr(line);
			ft_putstr("<<<<<\n");
			line = get_next_line (fd);
		}
	}
	else
	{
		ft_putstr("Error\n");
	}
	return (0);
}
