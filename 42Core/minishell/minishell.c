/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 15:35:02 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 15:44:16 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_only_cmd(t_shell *tab, char **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		if (execute_cmd(tab, &cmds[i]))
			break ;
		i++;
	}
	return (1);
}

void	exec_parsed_cmdlst(t_shell *table, char **cmds, t_token **tk_lst)
{
	int	i;
	int	start;

	start = 0;
	i = 0;
	if (!is_chained_command(tk_lst))
		execute_only_cmd(table, cmds);
	else
	{
		set_initial_input(table);
		i = in_redirected(cmds, table);
		if (i == -1)
			return ;
		start = i;
		while (cmds[i])
		{
			if (ft_strcmp(cmds[i], "|") == 0)
				start = piped_cmd(table, cmds, start, i);
			else if (!cmds[i + 1])
				last_piped_cmd(table, cmds, start, i);
			i++;
		}
		restore_initial_input(table);
	}
	wait_for_child(table->proc.child);
}

static void	run_cmd(char *option, t_shell *table, t_token *token_lst)
{
	if (!valid_quote(option))
	{
		printf("minishell: unclosed quotes\n");
		g_shell.error = 1;
		return ;
	}
	set_cmd_lst(table, token_lst, option);
	unlink(TMP_PATH);
}

static void	initialization(t_shell *shell, char **environ)
{
	envtable(shell, environ);
	shell->running = 1;
	shell->proc.pid = 0;
	g_shell.error = 0;
	g_shell.env = NULL;
	g_shell.path = NULL;
	shell->proc.child = 0;
	shell->proc.tmpin = 0;
	shell->proc.tmpout = 0;
	shell->proc.fdin = 0;
	shell->proc.fdout = 0;
	init_term();
}

int	main(int argc, char **argv, char **environ)
{
	t_shell	*shell;
	t_token	*token_lst;
	char	*input;	

	(void)argc;
	shell = create_table(CAPACITY);
	initialization(shell, environ);
	while (shell->running)
	{
		token_lst = NULL;
		input = NULL;
		if (argv[1])
		{
			input_argv(argv, &input);
			run_cmd(input, shell, token_lst);
			break ;
		}
		if (get_input(shell, &input))
			run_cmd(input, shell, token_lst);
		if (input)
			free(input);
	}
	free_table(shell);
	exit(g_shell.error);
}
