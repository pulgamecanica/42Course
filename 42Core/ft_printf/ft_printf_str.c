/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 16:50:55 by arosado-          #+#    #+#             */
/*   Updated: 2021/11/03 01:25:26 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static void	putstrfree(char *str)
{
	ft_putstr_fd(str, 1);
	free(str);
}

int	ft_fstr(char *str, t_flags *flags)
{
	int		total;
	char	*temp2;

	if (str)
		temp2 = ft_strdup(str);
	else
		temp2 = ft_strdup("(null)");
	if (flags->flag_dot && ft_strlen(temp2) > flags->maxfw)
		temp2 = ft_strfreesub(str, flags->maxfw, temp2);
	if (flags->minfw > ft_strlen(temp2))
	{
		if (flags->flag_minus)
			ft_putstr_fd(temp2, 1);
		putstrfree(ft_getprefix(' ', flags->minfw - ft_strlen(temp2)));
		if (!flags->flag_minus)
			ft_putstr_fd(temp2, 1);
		free(temp2);
		return (flags->minfw);
	}
	else
		ft_putstr_fd(temp2, 1);
	total = ft_strlen(temp2);
	free(temp2);
	return (total);
}
