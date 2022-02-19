/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 23:10:36 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/24 23:13:38 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	count_conversions(char *str)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	if (str == NULL)
		return (0);
	while (str[i] != '\0')
	{
		if (ft_strchr("%", str[i++]))
			if (str[i] != '\0')
				if (ft_strchr("cspdiuxX", str[i++]))
					result++;
	}
	return (result);
}

int	unsigned_num_counter(unsigned int nb, int base)
{
	int	total;

	total = 0;
	if (nb == 0)
		return (1);
	while (nb != 0)
	{
		nb /= base;
		total++;
	}
	return (total);
}

void	ft_recursiveaddr(unsigned long int nb, int is_recursive)
{
	char	*c;
	char	res;

	if (!is_recursive)
		write(1, "0x", 2);
	if (nb >= 16)
	{
		ft_recursiveaddr(nb / 16, 1);
		nb = nb % 16;
	}
	if (nb > 9)
		res = 'a' + (nb - 10);
	else
		res = nb + '0';
	c = &res;
	write (1, c, 1);
}

void	ft_putunsignednbr_fd(unsigned int nb, int fd)
{
	if (nb < 10)
	{
		nb = nb + '0';
		ft_putchar_fd(nb, fd);
	}
	else
	{
		ft_putunsignednbr_fd (nb / 10, fd);
		nb = nb % 10 + '0';
		ft_putchar_fd(nb, fd);
	}
}

int	ft_putstrnullcheck_fd(char *str, int fd)
{
	if (str == NULL)
	{
		ft_putstr_fd("(null)", fd);
		return (6);
	}
	else
		ft_putstr_fd(str, fd);
	return (ft_strlen(str));
}
