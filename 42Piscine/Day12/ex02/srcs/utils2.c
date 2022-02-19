/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 04:16:16 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/30 04:16:18 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "file_reader.h"

int	inside_str(char c, char *string)
{
	while (*string != '\0')
	{
		if (*string++ == c)
			return (1);
	}
	return (0);
}

void	ft_tailheader(char *filename)
{
	ft_putstr(1, "==> ");
	ft_putstr(1, filename);
	ft_putstr(1, " <==\n");
}

int	ft_atoi(char *str)
{
	int	result;
	int	helper;
	int	valid;

	result = 0;
	valid = 0;
	helper = 1;
	while (inside_str(*str, "\t\n\v\f\r "))
		str++;
	while (inside_str(*str, "-+"))
		if (*str++ == '-')
			helper *= -1;
	while (*str <= '9' && *str >= '0')
	{
		valid = 1;
		result *= 10;
		result += *str - '0';
		str++;
	}
	if (!valid)
		return (-1);
	return (result * helper);
}
