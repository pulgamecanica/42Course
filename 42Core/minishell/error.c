/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 12:56:36 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 15:45:03 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_msg(int type)
{
	if (type == QUOTES || type == D_QUOTES)
		printf("minishell: unclosed quotes\n");
	else if (type == EQUAL)
		printf("minishell: not found\n");
	else if (type == PIPE)
		printf ("minishell: parse error near \'|\'\n");
	g_shell.error = 1;
	return (-1);
}

static int	exit_iden_error(void)
{
	printf("minshell: \'=\' : not valid identifier\n");
	g_shell.error = 1;
	return (g_shell.error);
}

int	invalid_assig(t_token **tk_lst, t_shell *table)
{
	t_token	*tk;

	tk = *tk_lst;
	while (tk)
	{
		if (tk->prev && !ft_strcmp(tk->prev->value, "export")
			&& tk->type == DOLLAR && !expanded_var(tk->value, table))
		{
			if (tk->next && tk->next->type == EQUAL)
				return (exit_iden_error());
		}
		tk = tk->next;
	}
	return (0);
}

void	exit_process(t_shell *tab)
{
	free_cmd_lst(g_shell.path);
	free_cmd_lst(g_shell.env);
	free_table(tab);
	_exit(127);
}
