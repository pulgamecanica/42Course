/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 12:58:43 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 14:08:08 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_home(t_shell *table)
{
	int		i;
	t_vars	*item;

	i = get_position_env(table, "HOME");
	item = table->env_vars[i];
	if (item)
	{
		if (chdir(item->value) != 0)
		{
			printf("cd: no such file or directory: %s \n", item->value);
			g_shell.error = 1;
		}
	}
	else
	{
		printf("cd: HOME not set\n");
		g_shell.error = 1;
	}
}

int	count_cmds(char **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
		i++;
	return (i);
}

int	define_exit_code(int code)
{
	int	res;
	int	input_code;

	res = 0;
	input_code = code;
	if (DEFAULT_CODE < input_code)
	{
		res = input_code - DEFAULT_CODE;
		while (res > 255)
			res -= DEFAULT_CODE;
	}
	else if (DEFAULT_CODE > input_code)
	{
		res = DEFAULT_CODE + input_code;
		while (res < 0)
			res += DEFAULT_CODE;
	}
	return (res);
}

int	split_equal(char *input, int i, t_token **token_lst)
{
	if (i - 1 >= 0)
		pass_to_token(input, i - 1, token_lst);
	add_token(token_lst, create_token("=", EQUAL));
	return (1);
}
