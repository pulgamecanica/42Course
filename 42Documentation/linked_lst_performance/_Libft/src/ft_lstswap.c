/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstswap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 14:38:44 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/20 21:37:29 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_lstswap(t_list *lst1, t_list *lst2)
{
	void *	tmp;

	if (!lst1 || !lst2)
		return (0);
	tmp = lst1->content;
	lst1->content = lst2->content;
	lst2->content = tmp; 
	return (1);
}
