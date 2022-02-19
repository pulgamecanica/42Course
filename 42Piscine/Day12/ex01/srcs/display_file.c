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

void	cat_repeat(void)
{
	int		r_count;
	char	buf[BUF_SIZE + 1];

	r_count = 0;
	while (r_count >= 0)
	{
		r_count = read(0, buf, BUF_SIZE);
		buf[r_count] = '\0';
		ft_putstr(1, buf);
	}
}

void	display_content(char *file)
{
	int		fd;
	int		r_count;
	char	buf[BUF_SIZE + 1];

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		display_error(file);
		return ;
	}
	r_count = read(fd, buf, BUF_SIZE);
	while (r_count)
	{
		buf[r_count] = '\0';
		ft_putstr(1, buf);
		r_count = read(fd, buf, BUF_SIZE);
		if (r_count == -1)
			display_error(file);
	}
	if (close(fd) == -1)
		display_error(file);
}
