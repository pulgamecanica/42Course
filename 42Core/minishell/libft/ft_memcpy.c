/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 12:43:30 by bmachado          #+#    #+#             */
/*   Updated: 2021/02/23 12:30:06 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned int		a;
	unsigned char		*dest_i;
	unsigned const char	*src_i;

	if (dest == NULL && src == NULL)
		return (NULL);
	a = 0;
	dest_i = (unsigned char *)dest;
	src_i = (unsigned char *)src;
	while (a < n)
	{
		*((char *)dest_i + a) = *((char *)src_i + a);
		a++;
	}
	return (dest_i);
}
