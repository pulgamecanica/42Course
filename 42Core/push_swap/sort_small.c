#include "push_swap.h"

void	sort_three(t_stack **stack)
{
	if(get_max(*stack)->next == NULL)
	{
		if ((int)((*stack)->content) < (int)((*stack)->next->content))
			return ;
		else
		{
			sa(stack);
			return ;
		}
	}
	else if (get_max(*stack) == *stack)
	{
		ra(stack);
		sort_three(stack);
	}
	else
	{
		rra(stack);
		sort_three(stack);
	}
}

void	sort_small_stack(t_stack **a, t_stack **b)
{
	while(ft_lstsize(*a) > 3)
	{
		if ((ft_lstsize(get_min(*a)) < ((ft_lstsize(*a) + 1) / 2)))
			min_top(a, rra);
		else
			min_top(a, ra);
		if (is_sorted(*a))
			break ;
		if((int)((*a)->content) > (int)((*a)->next->content))
		{
			sa(a);
			continue ;
		}
		pb(a, b);
	}
	if (ft_lstsize(*a) == 3)
		sort_three(a);
	while(ft_lstsize(*b) != 0)
		pa(a, b);
	if (!is_sorted(*a))
		sort_small_stack(a, b);
}
