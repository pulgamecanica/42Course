/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 11:46:12 by arosado-          #+#    #+#             */
/*   Updated: 2021/12/14 11:46:14 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	sort(int ac, char **av)
{
	t_stack	*a;
	t_stack	*b;

	a = init_stack(ac, av, 0);
	if (a && ft_lstsize(a) > 1 && check_repeats(a, 0))
	{
		b = NULL;
		if (ft_lstsize(a) <= 3)
			sort_three(&a);
		else if (ft_lstsize(a) <= 10)
			sort_small_stack(&a, &b);
		else
		{
			sort_mean_to_b(&a, &b, ft_lstsize(a), get_mean(a));
			sort_three(&a);
			while (ft_lstsize(b))
			{
				place_best_top(&a, &b, get_cheapest_elem(&a, &b));
				pa(&a, &b);
			}
			min_top(&a, rra);
		}
	}
	ft_lstclear(&a, NULL);
	ft_lstclear(&b, NULL);
}

int	main(int ac, char **av)
{
	if (ac > 1)
	{
		sort(ac, av);
	}
	return (0);
}
