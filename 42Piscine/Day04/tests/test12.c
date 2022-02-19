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

void *ft_print_memory(void *addr, unsigned int size);

int     main(void)
{
        char            *c;

	c = "Lorem ipsum dolor\v sit amet, consectetur adipiscing elit. Proin eleifend id mauris in mollis. Mauris tristique vel mauris a ornare. Donec tempor consequat metus. Donec id lorem odio.\n";
        ft_print_memory(c, 181);
}
