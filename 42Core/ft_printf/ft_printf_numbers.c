/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_numbers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 18:03:17 by arosado-          #+#    #+#             */
/*   Updated: 2021/11/01 18:03:19 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_numlen(unsigned long int nbr, int baselen, int dot)
{
	int	total;

	total = 0;
	if (nbr == 0 && !dot)
		return (1);
	while (nbr != 0)
	{
		nbr /= baselen;
		total++;
	}
	return (total);
}

void	ft_putanynbr_fd(unsigned long int nbr, const char *base, int dot)
{
	if (nbr == 0 && dot)
		return ;
	if (nbr >= ft_strlen(base))
		ft_putanynbr_fd(nbr / ft_strlen(base), base, 0);
	ft_putchar_fd(base[nbr % ft_strlen(base)], 1);
}
