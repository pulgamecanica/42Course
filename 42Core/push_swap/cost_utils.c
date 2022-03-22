/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cost_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 17:09:55 by arosado-          #+#    #+#             */
/*   Updated: 2021/12/21 17:09:58 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	topr_cost(t_stack *stack, t_stack *elem)
{
	return (ft_lstsize(stack) - ft_lstsize(elem));
}

int	toprr_cost(t_stack *stack, t_stack *elem)
{
	(void)stack;
	return (ft_lstsize(elem));
}

t_stack	*element_after_me(t_stack **stack, t_stack *elem)
{
	t_stack	*temp;

	if ((int)(*stack)->content > (int)elem->content
		&& ((int)ft_lstlast(*stack)->content) < (int)elem->content)
		return (*stack);
	if ((int)get_min(*stack)->content > (int)elem->content)
		return (get_min(*stack));
	else if ((int)get_max(*stack)->content < (int)elem->content)
		return (get_max(*stack));
	temp = *stack;
	while (temp->next)
	{
		if (((int)temp->content < (int)elem->content)
			&& ((int)temp->next->content > (int)elem->content))
			break ;
		temp = temp->next;
	}
	if (temp->next)
		return (temp->next);
	else
		return (temp);
}

static int	align_elem_cost(t_stack **a, t_stack **b, t_stack *elem)
{
	int		b_mincost;
	int		a_mincost;
	t_stack	*a_elem;

	a_elem = element_after_me(a, elem);
	if (toprr_cost(*b, elem) < topr_cost(*b, elem))
		b_mincost = toprr_cost(*b, elem);
	else
		b_mincost = topr_cost(*b, elem);
	if (b_mincost > toprr_cost(*a, a_elem)
		|| b_mincost > topr_cost(*a, a_elem))
		return (b_mincost);
	if (toprr_cost(*a, a_elem) < topr_cost(*a, a_elem))
		a_mincost = toprr_cost(*a, a_elem);
	else
		a_mincost = topr_cost(*a, a_elem);
	return (a_mincost + b_mincost);
}

t_stack	*get_cheapest_elem(t_stack **a, t_stack **b)
{
	t_stack	*temp;
	t_stack	*res;
	int		cost;
	int		last_cost;

	temp = *b;
	last_cost = -1;
	while (temp)
	{
		cost = align_elem_cost(a, b, temp);
		if (cost < last_cost || last_cost == -1)
		{
			last_cost = cost;
			res = temp;
		}
		temp = temp->next;
	}
	return (res);
}
