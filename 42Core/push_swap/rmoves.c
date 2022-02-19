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

static void	rotate(t_stack **stack)
{
	t_stack *temp;
	t_stack *first;

	if (*stack && ft_lstsize(*stack) > 1)
	{
		first = *stack;
		temp = first;
		while (temp->next)
			temp = temp->next;
		*stack = first->next;
		temp->next = first;
		first->next = NULL;
	}
}

void	ra(t_stack **a)
{
	if (*a)
	{
		rotate(a);
		write(1, "ra\n", 3);
	}
	if (SHOW)
		print_stack(*a, "ra");
}

void	rb(t_stack **b)
{
	if (*b)
	{
		rotate(b);
		write(1, "rb\n", 3);
	}
	if (SHOW)
		print_stack(*b, "rb");
}

void	rr(t_stack **a, t_stack **b)
{

	rotate(a);
	rotate(b);
	write(1, "rr\n", 3);
	if (SHOW)
		print_both_stacks(*a, *b);
}
