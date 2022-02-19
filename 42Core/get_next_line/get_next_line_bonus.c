/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 16:28:08 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/23 16:32:07 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static char	*str;
	t_read		*r;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	str = (char *)malloc(sizeof(char));
	r = new_read();
	if (str == NULL || r == NULL)
		return (NULL);
	str[0] = 0;
	while (r->temp != '\n')
	{
		r->r_count = read(fd, &(r->temp), 1);
		if (r->r_count <= 0)
			break ;
		str = str_appendc(str, r->temp);
		r->r_total += r->r_count;
	}
	if (r->r_count < 0 || r->r_total == 0)
	{
		free(str);
		str = NULL;
	}
	free(r);
	return (str);
}
