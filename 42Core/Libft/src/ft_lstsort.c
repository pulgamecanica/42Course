/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 14:33:23 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/20 14:33:28 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static t_list * m_merge(t_list * a, t_list * b, int (*cmp)(void *, void *))
{
	t_list * result = NULL;

	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	if (cmp(a->content, b->content) > 0) {
		result = a;
		result->next = m_merge(a->next, b, cmp);
	}
	else {
		result = b;
		result->next = m_merge(a, b->next, cmp);
	}
	return (result);
}

static void m_split(t_list * source, t_list ** frontRef, t_list ** backRef)
{
	t_list * fast;
	t_list * slow;
	slow = source;
	fast = source->next;

	while (fast != NULL) {
		fast = fast->next;
		if (fast != NULL) {
			slow = slow->next;
			fast = fast->next;
		}
	}
	*frontRef = source;
	*backRef = slow->next;
	slow->next = NULL;
}

/**
 * MERGE SORT
 **/
void	ft_lstsort(t_list **lst, int (*cmp)(void *, void *))
{
	t_list * head = *lst;
	t_list * a;
	t_list * b;
	if ((head == NULL) || (head->next == NULL) || !cmp)
		return;
	m_split(head, &a, &b);
	ft_lstsort(&a, cmp);
	ft_lstsort(&b, cmp);
	*lst = m_merge(a, b, cmp);
}
