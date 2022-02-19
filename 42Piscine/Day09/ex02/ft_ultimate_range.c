/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultimate_range.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 12:38:56 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/22 12:39:02 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>

int	ft_ultimate_range(int **range, int min, int max)
{
	int	i;
	int	size;

	if (min >= max)
	{
		*range = 0;
		return (0);
	}
	size = max - min;
	*range = (int *)malloc((sizeof(int)) * (max - min));
	if (*range == NULL)
	{
		*range = 0;
		return (-1);
	}
	i = 0;
	while (i < size)
	{
		*(*range + i) = min + i;
		i++;
	}
	return (size);
}
