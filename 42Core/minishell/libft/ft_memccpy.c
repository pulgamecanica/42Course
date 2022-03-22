/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 12:48:28 by bmachado          #+#    #+#             */
/*   Updated: 2021/02/17 16:30:51 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	size_t			i;
	unsigned char	*dest_c;
	unsigned char	*src_c;

	src_c = (unsigned char *)src;
	dest_c = (unsigned char *)dest;
	i = 0;
	while (i < n && src_c[i] != (unsigned char)c)
	{
		dest_c[i] = src_c[i];
		i++;
	}
	if (i == n)
		return (NULL);
	else
	{
		dest_c[i] = src_c[i];
		i++;
		return (&dest_c[i]);
	}
}
