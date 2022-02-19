/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 16:01:25 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/12 16:01:28 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>

void	ft_div_mod(int a, int b, int *div, int *mod);

int	main(void)
{
	int	i1;
	int	i2;
	int	*div;
	int	*mod;

	i1 = 0;
	i2 = 0;
	div = &i1;
	mod = &i2;
	ft_div_mod(14, 6, div, mod);
	printf("Division: %u, Module: %u", *div, *mod);
}
