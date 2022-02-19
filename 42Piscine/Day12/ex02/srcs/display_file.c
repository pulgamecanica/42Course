/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 15:12:21 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/29 15:12:24 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "file_reader.h"

void	tail_repeat(void)
{
	int		r_count;

	r_count = 0;
	while (1)
		read(0, NULL, BUF_SIZE);
}

void	read_file(char *file, int start, int fd)
{
	int		r_count;
	char	buf[BUF_SIZE + 1];

	r_count = 1;
	while (r_count)
	{
		r_count = read(fd, buf, BUF_SIZE);
		if (r_count == -1)
			display_error(file);
		buf[r_count] = '\0';
		if (r_count >= start)
		{
			while (start < r_count)
			{
				if (start >= 0)
					ft_putchar(buf[start]);
				start++;
			}
		}
	}
}

void	display_content(char *file, int start, int ac, int current)
{
	int		fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		display_error(file);
		return ;
	}
	if (ac > 4)
		ft_tailheader(file);
	read_file(file, start, fd);
	if (close(fd) == -1)
		display_error(file);
	if (ac > 4 && current < ac - 1)
		ft_putstr(1, "\n");
}
