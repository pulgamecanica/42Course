/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  rrmoves.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:56:14 by arosado-          #+#    #+#             */
/*   Updated: 2021/12/14 14:56:19 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	revrotate(t_stack **stack)
{
	t_stack	*last;
	t_stack	*temp;

	temp = *stack;
	if (temp && ft_lstsize(*stack) > 1)
	{
		last = ft_lstlast(*stack);
		while (temp->next->next)
			temp = temp->next;
		temp->next = NULL;
		last->next = *stack;
		*stack = last;
	}
}

void	rra(t_stack **a)
{
	if (ft_lstsize(*a) > 1)
	{
		revrotate(a);
		write(1, "rra\n", 4);
	}
	if (SHOW)
		print_stack(*a, "rra");
}

void	rrb(t_stack **b)
{
	if (ft_lstsize(*b) > 1)
	{	
		revrotate(b);
		write(1, "rrb\n", 4);
	}
	if (SHOW)
		print_stack(*b, "rrb");
}

void	rrr(t_stack **a, t_stack **b)
{
	revrotate(a);
	revrotate(b);
	write(1, "rrr\n", 4);
	if (SHOW)
		print_both_stacks(*a, *b);
}
