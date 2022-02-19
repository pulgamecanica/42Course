/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_counters.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:40:45 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/25 16:40:46 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	count_chars(char c, int fd)
{
	ft_putchar_fd(c, fd);
	return (1);
}

int	count_nbr(int nbr, int fd)
{
	int	total;

	ft_putnbr_fd(nbr, fd);
	if (nbr == 0)
		return (1);
	total = 0;
	if (nbr == -2147483648)
		return (11);
	else if (nbr < 0)
	{
		nbr = -nbr;
		total++;
	}
	while (nbr != 0)
	{
		nbr /= 10;
		total++;
	}
	return (total);
}

int	count_unsigned(unsigned int nb, int fd)
{
	ft_putunsignednbr_fd(nb, fd);
	return (unsigned_num_counter(nb, 10));
}

int	ft_putaddr_fd(void *ptr, int fd)
{
	unsigned long int	temp;
	int					total;

	total = 2;
	temp = (unsigned long int)ptr;
	if (ptr == NULL || ptr == 0 || temp == 0)
	{
		write(fd, "0x0", 3);
		return (3);
	}
	while (temp != 0)
	{
		temp /= 16;
		total++;
	}
	ft_recursiveaddr((unsigned long int)ptr, 0);
	return (total);
}
