/*
 *	EXECUTE: gcc -o cgi_atoi_base cgl_atoi_base.c cgi_atoi_base.c -lcgl -I .
*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado <arosado-@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 12:19:25 by esali             #+#    #+#             */
/*   Updated: 2021/09/21 14:00:21 by esali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

static char	*rev_str(char *array, int len)
{
	char	temp;
	int		i;

	i = 0;
	while (i < len / 2)
	{
		temp = (char)array[i];
		*(array + i) = *(array + len - i - 1);
		*(array + len - i - 1) = temp;
		i++;
	}
	return (array);
}

static int	check_base(char *base)
{
	int	base_s;
	int	i;

	base_s = 0;
	while (base[base_s] != '\0')
	{
		if (base[base_s] == '-' || base[base_s] == '+')
			return (0);
		if (base[base_s] == ' ' || (base[base_s] >= 9 && base[base_s] <= 13))
			return (0);
		i = base_s + 1;
		while (base[i] != '\0')
		{
			if (base[base_s] == base[i])
				return (0);
			i ++;
		}
		base_s++;
	}
	if (base_s < 2)
		return (0);
	return (base_s);
}

static int	change_base(char str, char *base)
{
	int	j;

	j = 0;
	while (base[j] != '\0')
	{
		if (str == base[j])
		{
			return (j);
		}
		j++;
	}
	return (-1);
}

static int	ft_atoi(char *str, char *base, int base_size)
{
	int	nbr;
	int	minus;
	int	i;

	i = 0;
	minus = 1;
	nbr = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	while (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			minus *= (-1);
		i++;
	}
	while (change_base(str[i], base) >= 0)
	{
		nbr = (nbr * base_size) + change_base(str[i], base);
		i++;
	}
	return (minus * nbr);
}

int	ft_atoi_base(char *str, char *base)
{
	int	base_size;
	int	nbr;

	base_size = check_base(base);
	if (base_size == 0)
		return (0);
	nbr = ft_atoi(str, base, base_size);
	return (nbr);
}
