/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_stack.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 11:46:12 by arosado-          #+#    #+#             */
/*   Updated: 2021/12/14 11:46:14 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_stack	*init_stack_split(char **list, int checker)
{
	t_stack	*first;
	t_stack	*temp;
	int		i;

	i = 0;
	first = NULL;
	while (list[i] != NULL)
	{
		if (check_errors(list[i], checker))
			return (NULL);
		if (i == 0)
		{
			first = ft_newstack(ft_atoi(list[i]));
			first->next = temp;
			temp = first;
		}
		else
		{
			temp->next = ft_newstack(ft_atoi(list[i]));
			temp = temp->next;
		}
		i++;
	}
	temp->next = NULL;
	return (first);
}

t_stack	*init_stack(int ac, char **av, int checker)
{
	int		i;
	t_stack	*temp;
	t_stack	*first;

	if (ac == 2)
		return (init_stack_split(ft_split(av[1], ' '), checker));
	i = 0;
	while (++i < ac)
	{
		if (check_errors(av[i], checker))
			return (NULL);
		if (i == 1)
		{
			first = ft_newstack(ft_atoi(av[i]));
			first->next = temp;
			temp = first;
		}
		else
		{
			temp->next = ft_newstack(ft_atoi(av[i]));
			temp = temp->next;
		}
	}
	temp->next = NULL;
	return (first);
}
