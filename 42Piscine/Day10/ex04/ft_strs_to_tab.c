/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strs_to_tab.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 20:15:45 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/25 20:15:51 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "ft_stock_str.h"

char	*ft_strdup(char *src)
{
	int		n;
	char	*cpy;

	n = 0;
	while (src[n] != '\0')
		n++;
	cpy = (char *)malloc(sizeof(*src) * n + 1);
	if (cpy == NULL)
		return (0);
	n = 0;
	while (src[n] != '\0')
	{
		cpy[n] = src[n];
		n++;
	}
	cpy[n] = '\0';
	return (cpy);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

struct s_stock_str	*ft_strs_to_tab(int ac, char **av)
{
	t_stock_str	*stocks;
	t_stock_str	empty;
	int			i;

	i = 0;
	stocks = (t_stock_str *)malloc(sizeof(struct s_stock_str) * (ac + 1));
	if (stocks == NULL)
		return (NULL);
	while (i < ac)
	{
		stocks[i].size = ft_strlen(av[i]);
		stocks[i].str = av[i];
		stocks[i].copy = ft_strdup(av[i]);
		i++;
	}
	empty.size = 0;
	empty.copy = 0;
	empty.str = 0;
	stocks[i] = empty;
	return (stocks);
}
