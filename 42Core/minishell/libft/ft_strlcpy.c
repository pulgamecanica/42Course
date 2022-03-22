/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strlcpy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 14:07:23 by bmachado          #+#    #+#             */
/*   Updated: 2021/03/30 12:33:37 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dest_size)
{
	size_t	a;

	if (dst == NULL && src == NULL)
		return (0);
	a = 0;
	if (dest_size == 0)
		return (ft_strlen(src));
	while (a < dest_size - 1 && src[a])
	{
		dst[a] = src[a];
		a++;
	}
	if (a < dest_size)
		dst[a] = '\0';
	while (src[a])
		a++;
	return (a);
}
