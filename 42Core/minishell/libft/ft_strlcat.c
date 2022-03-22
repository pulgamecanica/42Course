/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 13:58:49 by bmachado          #+#    #+#             */
/*   Updated: 2021/03/12 12:01:06 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	c;
	size_t	a;
	size_t	b;

	if (size == 0)
		return (ft_strlen(src));
	else if (size < ft_strlen(dst))
		c = ft_strlen(src) + size;
	else
		c = ft_strlen(src) + ft_strlen(dst);
	a = 0;
	while (dst[a])
		a++;
	b = 0;
	while (src[b] && a < size - 1)
	{
		dst[a] = src[b];
		a++;
		b++;
	}
	dst[a] = '\0';
	return (c);
}
