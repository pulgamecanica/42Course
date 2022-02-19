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

void	ft_ultimate_ft(int *********nbr);

int	main(void)
{
	int	*********nbr;
	int	********nbr2;
	int	*******nbr3;
	int	******nbr4;
	int	*****nbr5;
	int	****nbr6;
	int	***nbr7;
	int	**nbr8;
	int	*nbr9;
	int	a;

	a = 0;
	nbr = &nbr2;
	nbr2 = &nbr3;
	nbr3 = &nbr4;
	nbr4 = &nbr5;
	nbr5 = &nbr6;
	nbr6 = &nbr7;
	nbr7 = &nbr8;
	nbr8 = &nbr9;
	nbr9 = &a;
	ft_ultimate_ft(nbr);
	printf("Memory Slot: %p, Value: %d", ********nbr, *********nbr);
}
