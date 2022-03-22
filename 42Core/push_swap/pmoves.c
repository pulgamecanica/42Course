/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rmoves.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:56:14 by arosado-          #+#    #+#             */
/*   Updated: 2021/12/14 14:56:19 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	pa(t_stack **a, t_stack **b)
{
	if (ft_lstsize(*b) > 0)
	{
		push(a, ft_lstnew(pop(b)));
		write(1, "pa\n", 3);
		if (SHOW)
			print_both_stacks(*a, *b);
	}
}

void	pb(t_stack **a, t_stack **b)
{
	if (ft_lstsize(*a) > 0)
	{
		push(b, ft_lstnew(pop(a)));
		write(1, "pb\n", 3);
		if (SHOW)
			print_both_stacks(*a, *b);
	}
}
