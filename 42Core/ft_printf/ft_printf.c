/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 23:08:35 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/24 23:08:37 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_printf(const char *str, ...)
{
	int		t;
	int		i;
	va_list	l;

	va_start(l, str);
	i = 0;
	t = 0;
	while (str[i] != '\0')
	{
		if (ft_strchr("%", str[i]))
		{
			i++;
			if (str[i] != '\0' && ft_strchr("cspdiuxX", str[i]))
			{
				t += (((int (*)())ft_getf(str[i++]))(va_arg(l, void *), 1));
				continue ;
			}
		}
		write(1, &str[i++], 1);
		t++;
	}
	va_end(l);
	return (t);
}
