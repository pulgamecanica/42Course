/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 16:01:25 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/12 16:01:28 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>

char *ft_strcapitalize(char *str);

int     main(void)
{
	char	*str_0;
	char	str[64];
	int	index;

	str_0 = "salut, comment tu vas ? 42mots quarante-deux; cinquante+et+un";
	index = 0;
	while(str_0[index])
	{
		str[index] = str_0[index];
		index++;
	}
	printf("Result: %s", ft_strcapitalize(str));
}
