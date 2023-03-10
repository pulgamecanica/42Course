/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 17:28:13 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/20 17:28:15 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstiter_param(t_list *lst, void (*f)(void *, void *), void * ptr)
{
	t_list	*temp;

	temp = lst;
	while (temp != NULL)
	{
		f(temp->content, ptr);
		temp = temp->next;
	}
}
