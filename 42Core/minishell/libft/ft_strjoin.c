/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 13:50:14 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/18 16:27:47 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static char	*alloc_join(const char *s1, const char *s2)
{
	int		ls1;
	int		ls2;
	int		full_size;
	char	*str;

	ls1 = ft_strlen(s1);
	ls2 = ft_strlen(s2);
	full_size = ls1 + ls2 + 1;
	str = (char *)malloc(sizeof(char) * (full_size));
	if (!str)
		return (NULL);
	return (str);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	int		i;
	int		ls1;
	char	*str;

	if (s1 && s2)
	{
		ls1 = ft_strlen(s1);
		str = alloc_join(s1, s2);
		if (!str)
			return (NULL);
		i = -1;
		while (s1[++i])
			str[i] = s1[i];
		i = -1;
		while (s2[++i])
		{
			str[ls1] = s2[i];
			ls1++;
		}
		str[ls1] = '\0';
		return (str);
	}
	else if (s1 && !s2)
		return ((char *)s1);
	return (NULL);
}
