/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_cmp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 14:07:19 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/20 14:07:21 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstadd_back_cmp(t_list **lst, t_list *new, int (*cmp)(void *, void *))
{
	t_list	*n;

	if (lst == NULL || *lst == NULL) {
		ft_lstadd_front(lst, new);
		return ;
	}
	if (cmp((*lst)->content, new->content) < 0) {
		ft_lstadd_front(lst, new);
	} else if ((*lst)->next == NULL) {
		ft_lstadd_back(lst, new);
	} else {
		n = *lst;
		while (n->next != NULL)
		{
			if (cmp((n->next)->content, new->content) < 0) {
				break;
			}
			n = n->next;
		}
		new->next = n->next;
		n->next = new;
	}
}
