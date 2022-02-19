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

void	ft_ultimate_div_mod(int *div, int *mod);

int	main(void)
{
	int	i1;
	int	i2;
	int	*div;
	int	*mod;

	i1 = 25;
	i2 = 7;
	div = &i1;
	mod = &i2;
	ft_ultimate_div_mod(div, mod);
	printf("Division: %u, Reminder: %u", *div, *mod);
}
