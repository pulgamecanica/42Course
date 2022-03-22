/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/25 15:24:18 by bmachado          #+#    #+#             */
/*   Updated: 2021/05/25 15:33:55 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*current;
	t_list	*head;

	if (!lst || !f || !del)
		return (NULL);
	head = ft_lstnew((f)(lst->content));
	if (!head)
		return (NULL);
	current = head;
	lst = lst->next;
	while (lst)
	{
		current->next = ft_lstnew((f)(lst->content));
		if (!(current->next))
		{
			ft_lstclear(&head, del);
			return (NULL);
		}
		lst = lst->next;
		current = current->next;
	}
	return (head);
}
