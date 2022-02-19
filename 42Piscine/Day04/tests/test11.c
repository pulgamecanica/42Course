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

void ft_putstr_non_printable(char *str);

int     main(void)
{
	ft_putstr_non_printable("Coucou\etu vas bien ?\0");
	printf("\n");
	ft_putstr_non_printable("\nCoucou\ntu vas bien ?");
	printf("\n");
	ft_putstr_non_printable("Coucou\vtu vas bien ?");
}
