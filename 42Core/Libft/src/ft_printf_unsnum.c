/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_unsnum.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 16:50:55 by arosado-          #+#    #+#             */
/*   Updated: 2021/11/01 16:50:59 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static t_numf	*get_format(unsigned int numlen, t_flags *flags)
{
	t_numf	*n;
	char	*temp;

	n = clean_format();
	if (flags->flag_dot && flags->maxfw > numlen)
	{
		temp = ft_getprefix('0', flags->maxfw - numlen);
		n->prezeros = ft_strfreedup(temp, n->prezeros);
		free(temp);
		numlen = flags->maxfw;
	}
	else if (flags->minfw > numlen && flags->flag_zero && !flags->flag_dot)
	{
		temp = ft_getprefix('0', flags->minfw - numlen);
		n->prewidth = ft_strfreedup(temp, n->prewidth);
		free(temp);
		numlen = flags->minfw;
	}
	if (flags->minfw > numlen)
	{
		temp = ft_getprefix(' ', flags->minfw - numlen);
		n->prewidth = ft_strfreedup(temp, n->prewidth);
		free(temp);
	}
	return (n);
}

int	ft_funsnum(unsigned int nbr, t_flags *flags)
{
	int				total;
	t_numf			*format;

	format = get_format(ft_numlen(nbr, 10, flags->flag_dot), flags);
	if (flags->flag_minus)
	{
		ft_putstr_fd(format->prezeros, 1);
		ft_putanynbr_fd(nbr, "0123456789", flags->flag_dot);
		ft_putstr_fd(format->prewidth, 1);
	}
	else
	{
		ft_putstr_fd(format->prewidth, 1);
		ft_putstr_fd(format->prezeros, 1);
		ft_putanynbr_fd(nbr, "0123456789", flags->flag_dot);
	}
	total = ft_strlen(format->prefix) + ft_strlen(format->prezeros)
		+ ft_strlen(format->prewidth) + ft_numlen(nbr, 10, flags->flag_dot);
	free_format(format);
	return (total);
}
