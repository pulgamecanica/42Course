/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_subarray.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:42:21 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/16 15:58:22 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	**ft_subarray(char **array, int start, int end)
{
	char	**sub_array;
	int		i;
	int		size;

	size = end - start;
	if (size < 0)
		size *= -1;
	sub_array = (char **)ft_calloc(sizeof(char *), (size + 1));
	if (!sub_array || !array)
		return (NULL);
	i = -1;
	while (++i < size)
	{
		sub_array[i] = array[start];
		start++;
	}
	sub_array[i] = NULL;
	return (sub_array);
}
