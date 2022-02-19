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

int ft_str_is_lowercase(char *str);

int     main(void)
{
	printf("Lower-Alpha String: %d\n", ft_str_is_lowercase("wealllovepulgaaaaz"));
	printf("Non-Lower-Alpha String: %d", ft_str_is_lowercase("thisisalmostalllowercaseZ"));
}
