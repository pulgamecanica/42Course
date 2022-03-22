/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_ptr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 16:50:55 by arosado-          #+#    #+#             */
/*   Updated: 2021/11/02 19:40:37 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static t_numf	*get_format(unsigned int numlen, t_flags *flags)
{
	t_numf	*n;

	n = clean_format();
	set_prefix(n, ft_strdup("0x"));
	if (flags->flag_dot && flags->maxfw > numlen)
	{
		set_prezeros(n, ft_getprefix('0', flags->maxfw - numlen));
		numlen = flags->maxfw;
	}
	else if (flags->minfw > numlen && flags->flag_zero && !flags->flag_dot)
	{
		set_prezeros(n, ft_getprefix('0', flags->minfw
				- numlen - ft_strlen(n->prefix)));
		numlen = flags->minfw;
	}
	if (flags->minfw > numlen + ft_strlen(n->prefix))
		set_prewidth(n, ft_getprefix(' ', flags->minfw
				- numlen - ft_strlen(n->prefix)));
	return (n);
}

int	ft_fptr(unsigned long int nbr, t_flags *flags)
{
	int				total;
	t_numf			*format;

	format = get_format(ft_numlen(nbr, 16, flags->flag_dot), flags);
	if (flags->flag_minus)
	{
		ft_putstr_fd(format->prefix, 1);
		ft_putstr_fd(format->prezeros, 1);
		ft_putanynbr_fd(nbr, "0123456789abcdef", flags->flag_dot);
		ft_putstr_fd(format->prewidth, 1);
	}
	else
	{
		ft_putstr_fd(format->prewidth, 1);
		ft_putstr_fd(format->prefix, 1);
		ft_putstr_fd(format->prezeros, 1);
		ft_putanynbr_fd(nbr, "0123456789abcdef", flags->flag_dot);
	}
	total = ft_strlen(format->prefix) + ft_strlen(format->prezeros)
		+ ft_strlen(format->prewidth) + ft_numlen(nbr, 16, flags->flag_dot);
	free_format(format);
	return (total);
}
