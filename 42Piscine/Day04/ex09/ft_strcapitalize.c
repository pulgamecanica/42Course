/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcapitalize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 18:58:59 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/13 18:59:04 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strlowcase(char *str)
{
	int	index;

	index = 0;
	while (str[index])
	{
		if (str[index] >= 'A' && str[index] <= 'Z')
			str[index] = str[index] + 32;
		index++;
	}
	return (str);
}

int	is_next_word(char c)
{
	if (c < 'a' || c > 'z')
		if (c < 'A' || c > 'Z')
			if (c < '0' || c > '9')
				return (1);
	return (0);
}

char	*ft_strcapitalize(char *str)
{
	int	next_word;
	int	index;

	next_word = 1;
	ft_strlowcase(str);
	index = 0;
	while (str[index])
	{
		if (is_next_word(str[index]))
		{
			index++;
			next_word = 1;
			continue ;
		}
		if ((str[index] >= 'a' && str[index] <= 'z') && next_word)
			str[index] = str[index] - 32;
		next_word = 0;
		index++;
	}
	return (str);
}
