/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 11:45:50 by arosado-          #+#    #+#             */
/*   Updated: 2021/12/14 11:45:52 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stack.h"
#include "ft_printf.h"
#include <stdint.h>
void	print_both_stacks(t_stack *s1, t_stack *s2)
{
	int	i;

	i = 0;
	ft_printf("*/----------------------\\*\n");
	ft_printf(" |	\033[0;36mA\033[0m	 \033[0;37mB\033[0m 	|\n");
	ft_printf(" | 	\033[0;36#\033[0m	 \033[0;37m#\033[0m	|\n");
	while (s1 || s2)
	{
		if (s1)
		{
			ft_printf(" | \033[0;36m% 6d\033[0m ", s1->content);
			s1 = s1->next;
		}
		else
			ft_printf(" |      ");
		if (s2)
		{
			ft_printf("	\033[0;37m%-6d\033[0m  |\n", s2->content);
			s2 = s2->next;
		}
		else
			ft_printf(" 		|\n");
	}
	ft_printf("\033[0m | –		      – |\n");
	ft_printf("*\\----------------------/*\n");
}

void	print_stack(t_stack *s, char *name)
{
	int	i;

	i = 0;
	ft_printf("*/----------% 3s---------\\*\n", name);
	while (s)
	{
		ft_printf(" |	% 4d: %-6d	|\n", i++, s->content);
		s = s->next;
	}
	ft_printf("*\\----------% 3s---------/*\n", name);
}

t_stack	*ft_newstack(int num)
{
	t_stack	*elem;

	elem = (t_list *)malloc(sizeof(t_stack));
	if (elem == NULL)
		return (NULL);
	elem->content = (void *)(uintptr_t) num;
	elem->next = NULL;
	return (elem);
}

void	push(t_stack **s, t_stack *new)
{
	if (s == NULL)
		return ;
	ft_lstadd_front(s, new);
}

int	*pop(t_stack **s)
{
	int		*content;
	t_stack	*poped;

	if (*s == NULL || s == NULL)
		return (NULL);
	poped = *s;
	*s = poped->next;
	content = poped->content;
	ft_lstdelone(poped, NULL);
	return (content);
}
