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

unsigned int ft_strlcpy(char *dest, char *src, unsigned int size);

int     main(void)
{
        char	*src;
	char	dest[19];
	int	result;
	src = "Bonjour avec 42 mon amis!";
	result = ft_strlcpy(dest, src, 18);
	printf("String: %s\n", src);
	printf("Copied[18]: %s\nSource lines[25]: %d", dest, result);
}
