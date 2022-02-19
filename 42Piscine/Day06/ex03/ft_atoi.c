/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 00:52:33 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/17 00:52:35 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	inside_str(char c, char *string)
{
	while (*string != '\0')
	{
		if (*string++ == c)
			return (1);
	}
	return (0);
}

int	ft_atoi(char *str)
{
	int	result;
	int	helper;

	result = 0;
	helper = 1;
	while (inside_str(*str, "\t\n\v\f\r "))
		str++;
	while (inside_str(*str, "-+"))
		if (*str++ == '-')
			helper *= -1;
	while (*str <= '9' && *str >= '0')
	{
		result *= 10;
		result += *str - '0';
		str++;
	}
	return (result * helper);
}
