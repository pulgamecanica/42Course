/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_max_min.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 23:45:35 by arosado-          #+#    #+#             */
/*   Updated: 2021/12/14 23:45:39 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	is_sorted(t_stack *stack)
{
	t_stack	*temp;

	temp = stack;
	while (temp->next != NULL)
	{
		if ((int)(temp->content) > (int)(temp->next->content))
			return (0);
		temp = temp->next;
	}
	return (1);
}

int	get_mean(t_stack *stack)
{
	t_stack	*temp;
	int		mean;

	temp = stack;
	mean = 0;
	while (temp != NULL)
	{
		mean += (int)(temp->content);
		temp = temp->next;
	}
	return (mean / ft_lstsize(stack));
}

t_stack	*get_max(t_stack *a)
{
	t_stack	*temp;
	t_stack	*result;

	temp = a;
	result = temp;
	if (a == NULL)
		return (NULL);
	while (temp != NULL)
	{
		if ((int)temp->content > (int)result->content)
			result = temp;
		temp = temp->next;
	}
	return (result);
}

t_stack	*get_min(t_stack *a)
{
	t_stack	*temp;
	t_stack	*result;

	temp = a;
	result = temp;
	if (a == NULL)
		return (NULL);
	while (temp != NULL)
	{
		if ((int)temp->content < (int)result->content)
			result = temp;
		temp = temp->next;
	}
	return (result);
}

void	min_top(t_stack **stack, void func(t_stack **))
{
	int	min;

	min = (int)get_min(*stack)->content;
	while ((int)(*stack)->content != min)
		func(stack);
}
