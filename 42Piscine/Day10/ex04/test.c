/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/26 14:34:06 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/26 14:34:08 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_stock_str.h"

struct s_stock_str	*ft_strs_to_tab(int ac, char **av);

void	print_stock(struct s_stock_str *stock, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		printf("StructStock: Size = %d, str: %s, copy: %s \n",
			stock->size, stock->str, stock->copy);
		stock++;
		i++;
	}
}

int	main(int c, char **a)
{
	print_stock(ft_strs_to_tab(c - 1, a + 1), c - 1);
}
