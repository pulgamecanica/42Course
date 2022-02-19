/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 21:25:47 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/28 21:25:49 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "file_reader.h"
#include <stdio.h>

void	display_error(char *file)
{
	ft_putstr(2, "tail: ");
	ft_putstr(2, file);
	ft_putstr(2, ": ");
	ft_putstr(2, strerror(errno));
	ft_putstr(2, "\n");
	errno = 0;
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int	ft_strcmp(char *s1, char *s2)
{
	char	c1;
	char	c2;

	while (1)
	{
		c1 = *s1++;
		c2 = *s2++;
		if (c1 < c2)
			return (-1);
		else if (c1 > c2)
			return (1);
		if (!c1)
			break ;
	}
	return (0);
}

int	ft_file_size(char *file)
{
	int		fd;
	int		r_count;
	int		file_size;
	char	buf[BUF_SIZE + 1];

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (-1);
	r_count = read(fd, buf, BUF_SIZE);
	file_size = r_count;
	while (r_count)
	{
		buf[r_count] = '\0';
		r_count = read(fd, buf, BUF_SIZE);
		file_size = file_size + r_count;
		if (r_count == -1)
			return (-1);
	}
	if (close(fd) == -1)
		return (-1);
	return (file_size);
}

void	ft_putstr(int fd, char *str)
{
	while (*str != '\0')
		write(fd, str++, 1);
}
