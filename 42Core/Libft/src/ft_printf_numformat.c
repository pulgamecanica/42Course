/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_numformat.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 01:28:47 by arosado-          #+#    #+#             */
/*   Updated: 2021/11/03 01:28:48 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

void	set_prefix(t_numf *n, char *str)
{
	free(n->prefix);
	n->prefix = ft_strdup(str);
	free(str);
}

void	set_prezeros(t_numf *n, char *str)
{
	free(n->prezeros);
	n->prezeros = ft_strdup(str);
	free(str);
}

void	set_prewidth(t_numf *n, char *str)
{
	free(n->prewidth);
	n->prewidth = ft_strdup(str);
	free(str);
}
