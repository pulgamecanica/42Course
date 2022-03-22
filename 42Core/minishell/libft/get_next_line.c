/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 15:38:45 by bmachado          #+#    #+#             */
/*   Updated: 2021/09/14 16:57:09 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*clear_txt(char *txt, char **line, int l)
{
	unsigned int	i;
	char			*temp;

	i = 0;
	while (txt[i])
	{
		if (txt[i] == '\n')
			break ;
		i++;
	}
	if (i < ft_strlen(txt))
	{
		*line = ft_substr(txt, 0, i);
		temp = ft_substr(txt, i + 1, ft_strlen(txt));
		free(txt);
		txt = ft_strdup(temp);
		free(temp);
	}
	else if (l == 0)
	{
		*line = txt;
		txt = NULL;
	}
	return (txt);
}

char	*save_txt(char *buffer, char *txt)
{
	char	*temp;

	if (txt)
	{
		temp = ft_strjoin(txt, buffer);
		free(txt);
		txt = ft_strdup(temp);
		free(temp);
	}
	else
		txt = ft_strdup(buffer);
	return (txt);
}

int	gnl(int fd, char **line)
{
	int			read_l;
	char		buffer[BUFFER_SIZE + 1];
	static char	*limit[4096];

	read_l = read(fd, buffer, BUFFER_SIZE);
	while (read_l)
	{
		if (read_l == -1 || line == NULL)
			return (-1);
		buffer[read_l] = '\0';
		limit[fd] = save_txt(buffer, limit[fd]);
		if (ft_strchr(buffer, '\n'))
			break ;
		read_l = read(fd, buffer, BUFFER_SIZE);
	}
	if (read_l <= 0 && !limit[fd])
	{
		*line = ft_strdup("");
		return (read_l);
	}
	limit[fd] = clear_txt(limit[fd], line, read_l);
	if (read_l <= 0 && !limit[fd])
		return (read_l);
	return (1);
}
