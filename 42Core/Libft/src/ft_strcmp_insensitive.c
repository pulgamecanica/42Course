/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp_insensitive.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 17:05:27 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/18 17:05:28 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

#define ft_max(x,y) ((x) > (y) ? (x) : (y));

int	ft_strcmp_insensitive(const char *s1, const char *s2)
{
	char	c1;
	char	c2;

	while (*s1 && *s2)
	{
		c1 = ft_tolower(*s1++);
		while (*s1 && !ft_isalnum(c1))
			c1 = ft_tolower(*s1++);
		c2 = ft_tolower(*s2++);
		while (*s2 && !ft_isalnum(c2))
			c2 = ft_tolower(*s2++);
		if (c1 != c2)
			return ((unsigned char)c1 - (unsigned char)c2);
		if (!c1 || !c2)
			break ;
	}
	return (0);
}
