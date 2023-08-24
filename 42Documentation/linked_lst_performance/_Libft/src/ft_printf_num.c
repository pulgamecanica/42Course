/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_num.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 16:50:55 by arosado-          #+#    #+#             */
/*   Updated: 2021/11/01 16:50:59 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static t_numf	*get_format(unsigned int l, t_flags *flags, int negative)
{
	t_numf	*n;

	n = clean_format();
	if (negative)
		n->prefix = ft_strfreedup("-", n->prefix);
	else if (flags->flag_plus)
		n->prefix = ft_strfreedup("+", n->prefix);
	else if (flags->flag_sp)
		n->prefix = ft_strfreedup(" ", n->prefix);
	if (flags->flag_dot && flags->maxfw > l)
	{
		set_prezeros(n, ft_getprefix('0', flags->maxfw - l));
		l = flags->maxfw;
	}
	if (negative)
		l++;
	if (flags->minfw > l && flags->flag_zero && !flags->flag_dot)
	{
		set_prezeros(n, ft_getprefix('0', flags->minfw - l));
		l = flags->minfw;
	}
	if (flags->minfw > l)
		set_prewidth(n, ft_getprefix(' ', flags->minfw - l));
	return (n);
}

int	ft_fnum(int num, t_flags *flags)
{
	int				total;
	t_numf			*format;
	unsigned int	nbr;

	nbr = num * (1 - ((num < 0) * 2));
	format = get_format(ft_numlen(nbr, 10, flags->flag_dot), flags, num < 0);
	if (flags->flag_minus)
	{
		ft_putstr_fd(format->prefix, 1);
		ft_putstr_fd(format->prezeros, 1);
		ft_putanynbr_fd(nbr, "0123456789", flags->flag_dot);
		ft_putstr_fd(format->prewidth, 1);
	}
	else
	{
		ft_putstr_fd(format->prewidth, 1);
		ft_putstr_fd(format->prefix, 1);
		ft_putstr_fd(format->prezeros, 1);
		ft_putanynbr_fd(nbr, "0123456789", flags->flag_dot);
	}
	total = ft_strlen(format->prefix) + ft_strlen(format->prezeros)
		+ ft_strlen(format->prewidth) + ft_numlen(nbr, 10, flags->flag_dot);
	free_format(format);
	return (total);
}
