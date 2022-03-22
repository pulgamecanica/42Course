/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 16:05:57 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 13:18:43 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_cd(t_shell *table, char **str)
{
	static char	*pwd;

	pwd = getcwd(NULL, 0);
	hash_insert_env(table, "OLDPWD", pwd);
	if (!str[1] || ft_strncmp(str[1], "~", 2) == 0)
		cd_home(table);
	else if (chdir(str[1]) != 0)
	{
		if (ft_isfilled(str[1]))
			printf("cd: no such file or directory: %s\n", str[1]);
		g_shell.error = 1;
		return ;
	}
	pwd = getcwd(NULL, 0);
	hash_insert_env(table, "PWD", pwd);
	g_shell.error = 0;
}

void	print_echo(char **str, t_shell *table)
{
	int		i;

	table->echo_flag = 0;
	if (!str[1])
	{
		printf("\n");
		g_shell.error = 0;
		return ;
	}
	i = 0;
	if (ft_strcmp(str[1], "-n") == 0)
	{
		i = 1;
		table->echo_flag = 1;
	}
	while (str[++i])
	{
		printf("%s", str[i]);
		if (str[i + 1])
			printf(" ");
	}
	if (table->echo_flag != 1)
		printf("\n");
	g_shell.error = 0;
}

void	print_pwd(char **cmd)
{
	char	*pwd;

	if (cmd[1] && ft_strcmp(cmd[1], "|") != 0)
	{
		printf("pwd: too many arguments\n");
		g_shell.error = 1;
		return ;
	}
	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
	g_shell.error = 0;
}

void	print_table_env(t_shell *table)
{
	int		i;
	t_vars	*current;

	i = 0;
	while (i < table->size)
	{
		current = table->env_vars[i];
		if (current && current->print)
			printf("%s=%s\n", current->key, current->value);
		i++;
	}
}

void	exit_function(t_shell *table, char **cmds)
{
	int	size;

	size = ft_arrlen(cmds);
	if (size > 2 && is_number(cmds[1]))
	{
		printf("exit: too many arguments\n");
		g_shell.error = 1;
		return ;
	}
	else if (size > 1)
	{
		if (!is_number(cmds[1]))
		{
			printf("minishell: exit: %s: numeric argument required\n", cmds[1]);
			g_shell.error = 255;
		}
		else if (ft_atoi(cmds[1]) < 0 || ft_atoi(cmds[1]) > 255)
			g_shell.error = define_exit_code(ft_atoi(cmds[1]));
		else
			g_shell.error = ft_atoi(cmds[1]);
	}
	printf("exit\n");
	table->running = 0;
}
