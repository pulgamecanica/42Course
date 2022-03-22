/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 13:39:58 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/18 17:23:45 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	input_argv(char **argv, char **input)
{
	int		i;
	char	*tmp_join;

	i = 1;
	if (!argv[i + 1])
		*input = argv[i];
	else
	{
		while (argv[i])
		{
			if (!argv[i + 1])
			{
				*input = ft_strjoin(*input, argv[i]);
				break ;
			}
			tmp_join = ft_strjoin(argv[i], " ");
			*input = ft_strdup(tmp_join);
			free(tmp_join);
			i++;
		}
	}
}

int	is_chained_command(t_token **tk_lst)
{
	t_token	*tk;

	tk = *tk_lst;
	while (tk)
	{
		if (tk->type == PIPE || redir_type(tk->type))
			return (1);
		tk = tk->next;
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

int	do_builtin(char **cmd, t_shell *table)
{
	if (ft_strcmp(cmd[0], "pwd") == 0)
		print_pwd(cmd);
	else if (ft_strcmp(cmd[0], "env") == 0)
	{
		if (cmd[1])
			return (0);
		print_table_env(table);
	}
	else if (ft_strcmp(cmd[0], "echo") == 0)
		print_echo(cmd, table);
	else if (ft_strcmp(cmd[0], "exit") == 0)
		exit_function(table, cmd);
	else if (ft_strcmp(cmd[0], "cd") == 0)
		handle_cd(table, cmd);
	else if (ft_strcmp(cmd[0], "export") == 0)
		export_function(cmd, table);
	else if (ft_strcmp(cmd[0], "unset") == 0)
		unset_function(table, cmd);
	return (1);
}

int	execute_cmd(t_shell *table, char **cmds)
{
	if (!cmds[0] || !check_pathexits(cmds[0]))
		return (0);
	if (is_builtin(cmds[0]))
		return (do_builtin(&cmds[0], table));
	else if (is_local_declare(cmds[0]) > 0)
		return (create_locals(cmds[0], table));
	else
		return (execute_shell(table, &cmds[0]));
	return (0);
}
