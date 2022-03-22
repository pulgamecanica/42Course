/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 15:18:19 by bmachado          #+#    #+#             */
/*   Updated: 2021/12/15 19:18:58 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_file(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '.' && str[i + 1] == '/')
			return (i);
		i++;
	}
	return (-1);
}

int	check_pathexits(char *str)
{
	int	ret;

	ret = is_file(str);
	if (ret < 0)
		return (-1);
	if (access(str, F_OK) == 0)
		return (1);
	else
		printf("minishell: No such file or directory: %s\n", str);
	g_shell.error = 127;
	return (0);
}
