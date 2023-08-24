/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 16:55:49 by arosado-          #+#    #+#             */
/*   Updated: 2021/11/01 16:55:51 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

char	*ft_getprefix(char c, int times)
{
	char	*str;

	str = (char *)malloc(sizeof(char) * (times + 1));
	str[times] = '\0';
	ft_memset(str, c, times * sizeof(char));
	return (str);
}

void	free_format(t_numf *format)
{
	free(format->prefix);
	free(format->prezeros);
	free(format->prewidth);
	free(format);
}

char	*ft_strfreedup(char *src, char *to_free)
{
	if (to_free)
		free(to_free);
	return (ft_strdup(src));
}

char	*ft_strfreesub(char *str, int end, char *to_free)
{
	if (to_free)
		free(to_free);
	if (str)
		return (ft_substr(str, 0, end));
	else
		return (ft_substr("(null)", 0, end));
}
