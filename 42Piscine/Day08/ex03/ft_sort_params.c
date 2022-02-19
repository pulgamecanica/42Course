/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_params.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/19 11:24:36 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/19 11:24:38 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>

void	put_strings(char **str, int size)
{
	int	i;
	int	j;

	i = 1;
	while (i < size)
	{
		j = 0;
		while (str[i][j] != '\0')
			write(1, &str[i][j++], 1);
		write(1, "\n", 1);
		i++;
	}
}

int	compare(char *str1, char *str2)
{
	while (*str1++ == *str2++)
		if (*str1 == '\0')
			return (-1);
	return (*--str1 - *--str2);
}

void	ft_swap(char **c1, char **c2)
{
	char	*c3;

	c3 = *c1;
	*c1 = *c2;
	*c2 = c3;
}

int	main(int ac, char **av)
{
	int	index;

	if (ac > 1)
	{
		index = 1;
		while (index < ac - 1)
		{
			if (compare(av[index], av[index + 1]) > 0)
			{
				ft_swap(&av[index], &av[index + 1]);
				index = 1;
				continue ;
			}
			index++;
		}
	}
	put_strings(av, ac);
	return (0);
}
