/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 22:51:39 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/18 22:51:41 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void		ft_lstdelelem(t_list **lst, t_list *elem, void (*del)(void *))
{
	t_list *tmp;
	t_list *last;

	if (!lst)
		return;
	tmp = *lst;
	last = NULL;
	while (tmp)
	{
		if (tmp == elem) {
			if (last)
				last->next = tmp->next;
			else
				*lst = tmp->next;
			ft_lstdelone(tmp ,del);
			break;
		}
		last = tmp;
		tmp = tmp->next;
	}

}
