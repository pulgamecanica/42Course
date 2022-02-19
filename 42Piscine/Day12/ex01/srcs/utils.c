/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 21:25:47 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/28 21:25:49 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "file_reader.h"

void	display_error(char *file)
{
	ft_putstr(2, "cat: ");
	ft_putstr(2, file);
	ft_putstr(2, ": ");
	ft_putstr(2, strerror(errno));
	ft_putstr(2, "\n");
	errno = 0;
}

void	ft_putstr(int fd, char *str)
{
	while (*str != '\0')
		write(fd, str++, 1);
}
