/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 15:40:35 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/22 21:35:44 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>

char	*rev_str(char *array, int len);
int		ft_atoi_base(char *str, char *base);
int		count_digits(long nb, int base_size);

int	num_length(int nb, int size)
{
	int	i;

	i = 0;
	if (nb < 0)
		i++;
	nb *= 1 - ((nb < 0) * 2);
	while (nb / size > 0)
	{
		nb /= size;
		i++;
	}
	return (i);
}

char	*helper(long n, char *base, int len, char *array)
{
	n *= 1 - ((n < 0) * 2);
	if (n >= len)
		helper(n / len, base, len, array + 1);
	*array = base[n % len];
	return (&array[0]);
}

int	check_reps(char *base, int index)
{
	int	i;

	i = index + 1;
	while (base[i] != '\0')
	{
		if (base[index] == base[i])
			return (1);
		if (base[index] == '-' || base[index] == '+')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_convert_base(char *nbr, char *base_from, char *base_to)
{
	long	decimal;
	int		i;
	int		len;
	char	*array;

	i = 0;
	array = NULL;
	while (base_to[i] != '\0')
		if (check_reps(base_to, i++))
			return (array);
	decimal = (long)ft_atoi_base(nbr, base_from);
	len = num_length(decimal, i);
	array = (char *)malloc(sizeof(char) * (len + 2));
	if (array == 0)
		return (0);
	if (i > 1 && array != NULL)
	{
		if (decimal < 0)
			array[len] = '-';
		array[len + 1] = '\0';
		array = helper(decimal, base_to, i, array);
		return (rev_str(&array[0], len + 1));
	}
	return (0);
}
