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

char *ft_strupcase(char *str);

int     main(void)
{
	char	*str;
	char	str_2[32];
	int	index;

	str = "Thanks for the Tests Pulga! <3!";
	index = 0;
	while (str[index])
	{
		str_2[index] = str[index];
		index++;
	}
	printf("String: %s\n", str);
	printf("Upper Case: %s", ft_strupcase(str_2));
}
