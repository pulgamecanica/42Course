/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smoves.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 12:11:39 by arosado-          #+#    #+#             */
/*   Updated: 2021/12/14 12:11:43 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	swap(t_stack **stack)
{
	t_stack	*first;
	t_stack	*second;
	t_stack	*third;

	first = *stack;
	second = (*stack)->next;
	third = second->next;
	*stack = second;
	second->next = first;
	first->next = third;
}

void	sa(t_stack **a)
{
	if (ft_lstsize(*a) > 1)
	{
		swap(a);
		write(1, "sa\n", 3);
		if (SHOW)
			print_stack(*a, "sa");
	}
}

void	sb(t_stack **b)
{
	if (ft_lstsize(*b) > 1)
	{
		swap(b);
		write(1, "sb\n", 3);
		if (SHOW)
			print_stack(*b, "sb");
	}
}

void	ss(t_stack **a, t_stack **b)
{
	if (ft_lstsize(*a) > 1)
		swap(a);
	if (ft_lstsize(*b) > 1)
		swap(b);
	write(1, "ss\n", 3);
	if (SHOW)
	{
		print_both_stacks(*a, *b);
	}
}
