/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 12:07:10 by bmachado          #+#    #+#             */
/*   Updated: 2021/02/17 12:07:13 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *c, size_t n)
{
	unsigned char	*dest;
	size_t			a;

	dest = (unsigned char *)c;
	a = 0;
	while (a < n)
	{
		dest[a] = 0;
		a++;
	}
}
