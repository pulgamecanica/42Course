/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 11:04:18 by arosado-          #+#    #+#             */
/*   Updated: 2021/12/14 11:04:21 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_H
# define STACK_H
#include "libft.h"


typedef t_list	t_stack;

int		*pop(t_stack **s);
int		get_mean(t_stack *stack);
int		is_sorted(t_stack *stack);
t_stack	*get_max(t_stack *a);
t_stack	*get_min(t_stack *a);
t_stack	*ft_newstack(int num);
t_stack	*init_stack_split(char **list, int checker);
t_stack	*init_stack(int ac, char **av, int checker);
void	print_stack(t_stack *s, char *name);
void	print_both_stacks(t_stack *s1, t_stack *s2);
void	push(t_stack **s, t_stack *element);
void	min_top(t_stack **stack, void func(t_stack **));

#endif
