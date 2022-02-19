/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 11:04:40 by arosado-          #+#    #+#             */
/*   Updated: 2021/12/14 11:04:42 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H
# ifndef SHOW
#  define SHOW 0
# endif
# include "ft_printf.h"
# include "get_next_line.h"
# include "stack.h"

t_stack		*get_cheapest_elem(t_stack **a, t_stack **b);
t_stack		*element_after_me(t_stack **stack, t_stack *elem);
void	sa(t_stack **a);
void	sb(t_stack **b);
void	ss(t_stack **a, t_stack **b);
void	pa(t_stack **a, t_stack **b);
void	pb(t_stack **a, t_stack **b);
void	ra(t_stack **a);
void	rb(t_stack **b);
void	rr(t_stack **a, t_stack **b);
void	rra(t_stack **a);
void	rrb(t_stack **b);
void	rrr(t_stack **a, t_stack **b);
void	sort_three(t_stack **stack);
void	sort_small_stack(t_stack **a, t_stack **b);
void	sort_mean_to_b(t_stack **a, t_stack **b, int mean, int len, int abs_mean);
void	place_best_top(t_stack **a, t_stack **b, t_stack *elem);
int	check_repeats(t_stack *stack, int checker);
int	check_errors(char *str, int checker);
int	topr_cost(t_stack *stack, t_stack *elem);
int	toprr_cost(t_stack *stack, t_stack *elem);

#endif
