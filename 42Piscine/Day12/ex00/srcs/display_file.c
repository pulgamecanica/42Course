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

void	display_content(char *file)
{
	int		fd;
	int		r_count;
	char	buf[BUF_SIZE + 1];

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr(OPEN_FILE_ERROR);
		return ;
	}
	r_count = read(fd, buf, BUF_SIZE);
	while (r_count)
	{
		buf[r_count] = '\0';
		ft_putstr(buf);
		r_count = read(fd, buf, BUF_SIZE);
		if (r_count == -1)
			ft_putstr(READ_FILE_ERROR);
	}
	if (close(fd) == -1)
		ft_putstr(CLOSE_FILE_ERROR);
}
