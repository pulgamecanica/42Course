/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_f.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 16:39:37 by arosado-          #+#    #+#             */
/*   Updated: 2021/11/01 16:39:38 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	*ft_f(char c)
{
	if (c == 'c')
		return (ft_fchar);
	else if (c == 's')
		return (ft_fstr);
	else if (c == 'd')
		return (ft_fnum);
	else if (c == 'i')
		return (ft_fnum);
	else if (c == 'p')
		return (ft_fptr);
	else if (c == 'u')
		return (ft_funsnum);
	else if (c == 'x')
		return (ft_fhex);
	else if (c == 'X')
		return (ft_fhexupper);
	return (NULL);
}
