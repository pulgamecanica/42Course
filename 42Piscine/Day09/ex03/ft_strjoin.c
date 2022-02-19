/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 13:13:08 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/22 13:13:13 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>

int	ft_strlen(char *str)
{
	int	result;

	result = 0;
	while (*str++ != '\0')
		result++;
	return (result);
}

int	calc_len(int size, char **strs, char *sep)
{
	int	index;
	int	count;

	index = 0;
	count = 0;
	while (index < size)
		count += (ft_strlen(strs[index++]));
	count += (ft_strlen(sep) * (index));
	return (count);
}

char	*ft_strncat(char *dest, char *src, unsigned int nb)
{
	char	*dst;

	dst = dest;
	while (*dst != '\0')
		dst++;
	while (*src != '\0' && nb > 0)
	{
		*dst = *(unsigned char *)src;
		dst++;
		src++;
		nb--;
	}
	*dst = '\0';
	return (dest);
}

char	*ft_strcpy(char *dest, char *src)
{
	int	index;

	index = 0;
	while (src[index] != '\0')
	{
		dest[index] = src[index];
		index++;
	}
	dest[index] = '\0';
	return (dest);
}

char	*ft_strjoin(int size, char **strs, char *sep)
{
	char	*array;
	char	*helper;
	int		index;

	if (size == 0)
		return ((char *)malloc(sizeof(char)));
	array = (char *)malloc(sizeof(char) * calc_len(size, strs, sep));
	if (array == NULL)
		return (0);
	index = 0;
	helper = array;
	while (index < size)
	{
		helper += ft_strlen(ft_strcpy(helper, strs[index]));
		if (index < size - 1)
			helper += ft_strlen(ft_strcpy(helper, sep));
		index++;
	}
	*helper = '\0';
	return (array);
}
