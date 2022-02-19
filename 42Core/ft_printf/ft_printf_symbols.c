/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_symbols.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 15:08:06 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/25 15:08:09 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

void	(*ft_getf(char c))
{
	if (c == 'c')
		return (&count_chars);
	else if (c == 's')
		return (&ft_putstrnullcheck_fd);
	else if (c == 'd')
		return (&count_nbr);
	else if (c == 'i')
		return (&count_nbr);
	else if (c == 'p')
		return (&ft_putaddr_fd);
	else if (c == 'u')
		return (&count_unsigned);
	else if (c == 'x')
		return (&count_hexnum);
	else if (c == 'X')
		return (&count_hexnumupper);
	return (NULL);
}
