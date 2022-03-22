/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  file_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dareias- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 08:24:28 by arosado-          #+#    #+#             */
/*   Updated: 2022/03/20 10:03:12 by dareias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

int	file_exists(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		printf("File [%s] doesn't exist\n", filename);
		return (-1);
	}
	close(fd);
	return (1);
}

char	*get_next_line_no_nl(char *line)
{
	char	*tmp;

	tmp = line;
	line = ft_strndup(tmp, ft_strlen(tmp) - 1);
	free(tmp);
	return (line);
}
