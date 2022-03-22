/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_flags.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 16:39:02 by arosado-          #+#    #+#             */
/*   Updated: 2021/11/01 17:52:12 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static int	get_num(const char *str, int *i)
{
	int	total;

	total = 0;
	while (ft_isdigit(str[*i]))
	{
		total *= 10;
		total += str[*i] - '0';
		(*i)++;
	}
	(*i)--;
	return (total);
}

static	t_flags	*initflags(void)
{
	t_flags	*f;

	f = (t_flags *)malloc(sizeof(t_flags) * 1);
	if (f == NULL)
		return (NULL);
	f->flag_sp = 0;
	f->flag_minus = 0;
	f->flag_zero = 0;
	f->flag_plus = 0;
	f->flag_dot = 0;
	f->flag_cardinal = 0;
	f->minfw = 0;
	f->maxfw = 0;
	return (f);
}

static void	set_flag(char c, t_flags *f)
{
	if (c == '0')
		f->flag_zero = 1;
	else if (c == ' ')
		f->flag_sp = 1;
	else if (c == '-')
		f->flag_minus = 1;
	else if (c == '+')
		f->flag_plus = 1;
	else if (c == '#')
		f->flag_cardinal = 1;
}

t_flags	*getfl(const char *str, int *i)
{
	t_flags	*f;

	f = initflags();
	while (ft_strchr("0123456789 -+#.", str[*i]))
	{
		set_flag(str[*i], f);
		if (str[*i] == '.')
		{
			f->flag_dot = 1;
			if (ft_isdigit(str[*i + 1]))
			{
				(*i)++;
				f->maxfw = get_num(str, i);
			}
		}
		else if (ft_isdigit(str[*i]))
			f->minfw = get_num(str, i);
		(*i)++;
	}
	return (f);
}

t_numf	*clean_format(void)
{
	t_numf	*n;

	n = (t_numf *)malloc(sizeof(t_numf));
	if (n == NULL)
		return (NULL);
	n->prefix = ft_strdup("");
	n->prezeros = ft_strdup("");
	n->prewidth = ft_strdup("");
	return (n);
}
