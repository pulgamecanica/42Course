/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 19:27:50 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/09 19:27:53 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>

void	ft_putnbr(int nb)
{
	if (nb < 0)
	{
		if (nb == -2147483648)
		{
			ft_putnbr (-21);
			ft_putnbr (47483648);
			return ;
		}
		write(1, "-", 1);
		nb = nb * -1;
	}
	if (nb >= 0 && nb < 10)
	{
		nb = nb + '0';
		write(1, &nb, 1);
	}
	else
	{
		ft_putnbr (nb / 10);
		nb = nb % 10 + '0';
		write(1, &nb, 1);
	}
}
