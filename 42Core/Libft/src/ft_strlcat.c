/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 22:48:05 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/20 22:48:08 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	unsigned int	temp;

	temp = ft_strlen(dest) + ft_strlen(src);
	if (ft_strlen(dest) < size)
	{
		size = size - ft_strlen(dest) - 1;
		dest = dest + ft_strlen(dest);
		while (*src != 0)
		{
			if (size <= 0)
				break ;
			else
				*dest++ = *src++;
			size--;
		}
		*dest = '\0';
		return (temp);
	}
	else
		return (ft_strlen(src) + size);
}
