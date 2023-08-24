/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 23:44:41 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/18 23:44:42 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strndup(const char *src, size_t l)
{
	int		n;
	int		len;
	char	*cpy;

	if (ft_strlen(src) < l)
		len = ft_strlen(src);
	else
		len = l;
	cpy = (char *)malloc(sizeof(*src) * len + 1);
	if (cpy == NULL)
		return (NULL);
	n = 0;
	while (n < len)
	{	
		cpy[n] = src[n];
		n++;
	}
	cpy[n] = '\0';
	return (cpy);
}
