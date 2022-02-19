/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 01:34:35 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/17 01:34:40 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	helper(long n, char *base, int len)
{
	if (n < 0)
		ft_putchar('-');
	n *= 1 - ((n < 0) * 2);
	if (n >= len)
		helper(n / len, base, len);
	ft_putchar(base[n % len]);
}

int	check_reps(char *base, int index)
{
	int	i;

	i = index + 1;
	while (base[i] != '\0')
	{
		if (base[index] == base[i])
			return (1);
		i++;
	}
	return (0);
}

void	ft_putnbr_base(int nbr, char *base)
{
	int	len;

	len = 0;
	while (base[len] != '\0')
	{
		if (base[len] == '-' || base[len] == '+')
			return ;
		if (check_reps(base, len))
			return ;
		len++;
	}
	if (len > 1)
		helper((long) nbr, base, len);
}
