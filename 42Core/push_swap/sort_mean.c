#include "push_swap.h"

static	int	shared_move(t_stack **a, t_stack **b, t_stack *elem, t_stack *after_me)
{
	if (topr_cost(*b, elem) < toprr_cost(*b, elem))
	{
		if (ft_lstsize(after_me) > ft_lstsize(*a) / 2 || topr_cost(*a, after_me) < topr_cost(*b, elem))
			rr(a, b);
		else
			return (0);
	}
	else
	{
		if (ft_lstsize(after_me) < ft_lstsize(*a) / 2 || toprr_cost(*a, after_me) < toprr_cost(*b, elem))
			rrr(a, b);
		else
			return (0);
	}
	return (1);
}

void	place_best_top(t_stack **a, t_stack **b, t_stack *elem)
{
	t_stack	*after_me;
	
	after_me = element_after_me(a, elem);
	while (after_me != *a && elem != *b)
	{
		if (shared_move(a, b, elem, after_me))
			continue ;
		break ;
	}
	while (after_me != *a)
	{
		if (toprr_cost(*a, after_me) < topr_cost(*a, after_me))
			rra(a);
		else
			ra(a);
	}
	while (elem != *b)
	{
		if (toprr_cost(*b, elem) < topr_cost(*b, elem))
			rrb(b);
		else
			rb(b);
	}
}

static void	insert_sorted_b(t_stack **a, t_stack **b, int mean)
{
	t_stack	*min_b;

	min_b = get_min(*b);
	pb(a, b);
	if (ft_lstsize(*b) > 1)
	{
		if ((int)(*b)->content < (int)min_b->content)
			rb(b);
		else if ((int)(*b)->content < mean)
			rb(b);
	}
}


void	sort_mean_to_b(t_stack **a, t_stack **b, int mean, int len, int abs_mean)
{
	if (len > 3)
	{
		if ((int)(*a)->content <= mean)
			insert_sorted_b(a, b, abs_mean);
		else
			ra(a);
		sort_mean_to_b(a, b, get_mean(*a), ft_lstsize(*a), abs_mean);
		return ;
	}
}
