/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_int_tab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 18:41:43 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/12 18:41:47 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	swap(int *a, int *b)
{
	int	helper;

	if (a && b)
	{
		helper = *a;
		*a = *b;
		*b = helper;
	}
}

void	sort(int *tab, int size)
{
	int	i;

	i = 0;
	while (i < size - 1)
	{
		if (*(tab + i) > *(tab + i + 1))
			swap((tab + i), (tab + i + 1));
		i++;
	}
}

void	ft_sort_int_tab(int *tab, int size)
{
	int	i;

	i = 0;
	while (1)
	{
		if (*(tab + i) > *(tab + i + 1))
		{
			sort(tab, size);
			i = 0;
			continue ;
		}
		i++;
		if (i == size)
			break ;
	}
}
