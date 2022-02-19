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

char *ft_strncpy(char *dest, char *src, unsigned int n);

int     main(void)
{
        char	*src;
//	char	dest[20];
	char	dest_0[20];

	src = "Awesome Pulgamecanica! ;)";
	printf("Source: %s\n", src);
//	ft_strncpy(dest, src, 9);
//	printf("Copied[9]: %s\n", dest);
	ft_strncpy(dest_0, src, 16);
	printf("Copied[7]: %s", dest_0);
}
