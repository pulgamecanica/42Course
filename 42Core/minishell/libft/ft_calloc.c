/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 12:08:12 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/16 17:58:02 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t mem, size_t size)
{
	char	*alloc;

	if (!mem || !size)
	{
		mem = 1;
		size = 1;
	}
	alloc = malloc(size * mem);
	if (!alloc)
		return (NULL);
	ft_memset(alloc, 0, size * mem);
	return (alloc);
}
