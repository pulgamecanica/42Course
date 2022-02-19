/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 11:38:39 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/17 11:38:43 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	invalid_base(char *base)
{
	int	i;
	int	result;

	result = 0;
	while (*base != '\0')
	{
		if (*base == '-' || *base == '+')
			return (0);
		if (*base == ' ' || (*base >= 9 && *base <= 13))
			return (0);
		i = result + 1;
		while (base[i] != '\0')
		{
			if (base[result] == base[i])
				return (0);
			i++;
		}
		result++;
	}
	if (result < 2)
		return (0);
	else
		return (result);
}

int	get_value(char *base, char c)
{
	int	index;

	index = 0;
	while (base[index] != '\0')
	{
		if (base[index] == c)
			return (index);
		index++;
	}
	return (-1);
}

int	get_size(char *base)
{
	int	index;

	index = -1;
	while (base[++index] != '\0')
	{
		if (base[index] == '-' || base[index] == '-' || base[index] <= 32)
			return (0);
		if (base[index + 1] == base[index])
			return (0);
	}
	if (index < 2)
		return (0);
	return (index);
}

int	ft_atoi_base(char *str, char *base)
{
	int	result;
	int	helper;
	int	size;

	result = 0;
	helper = 1;
	size = get_size(base);
	if (invalid_base(base))
	{
		return (0);
	}
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	while (*str == '-' || *str == '+')
	{
		if (*str == '-')
			helper *= (-1);
		str++;
	}
	while (get_value(base, *str) != -1)
	{
		result = (result * size) + get_value(base, *str);
		str++;
	}
	return (result * helper);
}
