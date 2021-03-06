/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 13:41:53 by bmachado          #+#    #+#             */
/*   Updated: 2021/12/15 13:57:58 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy(char *s1, const char *s2)
{
	int	i;

	if (!s1 && !s2)
		return (NULL);
	i = 0;
	while (s2[i] != '\0')
	{
		s1[i] = s2[i];
		i++;
	}
	while (s1[i] != '\0')
	{
		s1[i] = '\0';
		i++;
	}
	return (s1);
}
