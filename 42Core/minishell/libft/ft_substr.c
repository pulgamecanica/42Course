/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 14:49:27 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 10:51:21 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_substr(char const *s, unsigned int start, size_t n)
{
	size_t	a;
	size_t	b;
	char	*str;
	size_t	real_size;

	if (!s)
		return (NULL);
	real_size = ft_strlen(s);
	if (real_size < n)
		n = real_size;
	str = (char *)malloc(sizeof(char) * (n + 1));
	if (!str)
		return (NULL);
	a = -1;
	b = 0;
	while (s[++a])
	{
		if (a >= start && b < n)
		{
			str[b] = s[a];
			b++;
		}
	}
	str[b] = '\0';
	return (str);
}
