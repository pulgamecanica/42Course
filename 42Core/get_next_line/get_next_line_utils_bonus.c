/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 15:53:57 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/23 15:54:23 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line_bonus.h"

void	ft_putstr(char *str)
{
	while (*str != '\0')
		write(1, str++, 1);
}

int	str_len(char *str)
{
	int	i;

	i = 0;
	if (str == 0)
		return (0);
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*str_appendc(char *str, char c)
{
	char	*temp;
	int		i;

	temp = (char *)malloc(sizeof(char) * (str_len(str) + 2));
	if (temp == NULL)
		return (NULL);
	i = 0;
	if (str != 0)
	{
		while (str[i] != '\0')
		{
			temp[i] = str[i];
			i++;
		}
		free(str);
	}
	temp[i++] = c;
	temp[i] = '\0';
	return (temp);
}

t_read	*new_read(void)
{
	t_read	*new;

	new = (t_read *)malloc(sizeof(t_read));
	if (new == NULL)
		return (NULL);
	new->r_total = 0;
	new->r_count = 0;
	new->temp = '\0';
	return (new);
}
