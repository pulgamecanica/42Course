/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp_alnum.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 12:08:00 by arosado-          #+#    #+#             */
/*   Updated: 2023/09/06 12:08:00 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// This sorting is not quite correct...

int	ft_strcmp_alnum(const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s1[i] && s2[j])
	{
		if (ft_tolower(s1[i]) != ft_tolower(s2[j]))
			break;	
		i++;
		j++;
	}
	return (ft_tolower(s1[i]) - ft_tolower(s2[j]));
}
