/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 16:28:08 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/22 16:28:15 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

char	*find_last_index(char *str)
{
	char	*res;
	int		i;

	res = malloc(sizeof(char));
	if (res == NULL || str == NULL || ft_strlen(str) == 0)
		return (NULL);
	res[0] = '\0';
	i = 0;
	while (str[i] != '\0')
	{
		res = str_appendc(res, str[i]);
		if (str[i] == '\n' || str == NULL)
			break ;
		i++;
	}
	return (res);
}

char	*ft_strjoin(char *s1, char s2[])
{
	char	*new;
	int		i;
	int		k;

	if (s1 == NULL || s1 == NULL)
		return (NULL);
	new = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (new == NULL)
		return (NULL);
	i = -1;
	while (s1[++i] != '\0')
		new[i] = s1[i];
	k = -1;
	while (s2[++k] != '\0')
		new[i + k] = s2[k];
	new[i + k] = '\0';
	return (new);
}

void	init_staticstr(char *str)
{
	str = (char *)malloc(sizeof(char));
	str[0] = '\0';
}

char	*get_next_line(int fd)
{
	static char	*str;
	char		temp[BUFFER_SIZE + 1];
	char		*res;
	int			r_count;

	r_count = BUFFER_SIZE;
	init_staticstr(str);
	if (str == NULL)
		str = (char *)malloc(sizeof(char));
	if (fd < 0 || BUFFER_SIZE < 0 || str == NULL)
		return (NULL);
	while (!ft_strchr(str, '\n') && r_count == BUFFER_SIZE)
	{
		r_count = read(fd, &temp, BUFFER_SIZE);
		if (r_count == -1)
			return (NULL);
		else if (r_count == 0)
			break ;
		temp[r_count] = '\0';
		str = ft_strjoin(str, temp);
	}
	res = find_last_index(str);
	str += ft_strlen(res);
	return (res);
}
