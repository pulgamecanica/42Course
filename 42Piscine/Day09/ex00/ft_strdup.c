/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 12:38:09 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/22 12:38:21 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>

char	*ft_strdup(char *src)
{
	int		n;
	char	*cpy;

	n = 0;
	while (src[n] != '\0')
		n++;
	cpy = (char *)malloc(sizeof(*src) * n + 1);
	if (cpy == NULL)
		return (0);
	n = 0;
	while (src[n] != '\0')
	{	
		cpy[n] = src[n];
		n++;
	}
	cpy[n] = '\0';
	return (cpy);
}
