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
	if (ac < 2)
		ft_putstr(NO_ARGUMENT_ERROR);
	if (ac > 2)
		ft_putstr(TOO_MANY_ARGUMENTS_ERROR);
	if (ac == 2)
	{
		display_content(av[1]);
	}
	return (0);
}
