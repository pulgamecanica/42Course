/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 14:28:57 by bmachado          #+#    #+#             */
/*   Updated: 2021/03/12 12:01:23 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t len)
{
	size_t	size;

	if (*s2 == '\0')
		return ((char *)s1);
	size = ft_strlen(s2);
	while (*s1 && len-- >= size)
	{
		if (*s1 == *s2 && ft_strncmp(s1, s2, size) == 0)
			return ((char *)s1);
		s1++;
	}
	return (0);
}
