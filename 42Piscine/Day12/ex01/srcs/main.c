/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 15:12:53 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/29 15:12:55 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_reader.h"

int	main(int ac, char **av)
{
	int	i;

	if (ac < 2)
		cat_repeat();
	if (ac > 2)
	{
		i = 1;
		while (i < ac)
			display_content(av[i++]);
	}
	if (ac == 2)
	{
		display_content(av[1]);
	}
	return (0);
}
