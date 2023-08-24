/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 16:17:29 by arosado-          #+#    #+#             */
/*   Updated: 2021/11/01 16:17:31 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static int	handle_format(const char *str, int *i, va_list a)
{
	int		t;
	t_flags	*f;

	t = 0;
	f = getfl(str, i);
	if (f)
	{
		t = ((int (*)(void *, void *))ft_f(str[*(i)++]))(va_arg(a, void *), f);
		free(f);
	}
	return (t);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	int		t;
	va_list	a;

	va_start(a, str);
	i = 0;
	t = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '%')
		{
			if (ft_strchr("cspiduxX0123456789 +-.#", str[++i]))
			{
				t += handle_format(str, &i, a);
				i++;
				continue ;
			}
		}
		ft_putchar_fd(str[i], 1);
		t++;
		i++;
	}
	va_end(a);
	return (t);
}
