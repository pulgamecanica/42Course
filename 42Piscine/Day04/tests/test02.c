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

int ft_str_is_alpha(char *str);

int     main(void)
{
	printf("Alpha String: %d\n", ft_str_is_alpha("ThisstringisSOoooalpha"));
	printf("Non-Alpha String: %d", ft_str_is_alpha("This is N0T A1Ph4 At A11"));
}
