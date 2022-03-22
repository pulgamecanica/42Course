/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:51:23 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 13:16:59 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	can_execute(char *cmd)
{
	struct stat	sb;

	if (stat(cmd, &sb) != 0)
		return (0);
	return (1);
}

static int	exec_error(int ret, char **path, char *cmd, char *path_item)
{
	if (ret != 0 || !path || !path_item)
	{
		printf("minishell: command not found: %s\n", cmd);
		return (1);
	}
	return (0);
}

static char	**find_path_env(t_shell *table)
{
	int		i;
	t_vars	*item;
	char	*tmp;

	i = get_position_env(table, "PATH");
	item = table->env_vars[i];
	if (item)
	{
		tmp = item->value;
		g_shell.path = ft_split(tmp, ':');
	}
	return (g_shell.path);
}

static int	cmd_location(char **path, char **cmd)
{
	int		i;
	char	*tmp;

	i = 0;
	if (!path)
		return (0);
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(tmp, cmd[0]);
		free(tmp);
		if (can_execute(path[i]))
			break ;
		i++;
	}
	return (i);
}

int	execute_shell(t_shell *tab, char **cmds)
{
	int			i;
	static int	ret;

	find_path_env(tab);
	minishell_env(tab);
	i = cmd_location(g_shell.path, cmds);
	tab->proc.pid = fork();
	tab->proc.child++;
	exec_signal();
	if (tab->proc.pid == 0)
	{
		if (!cmds[0])
			exit(0);
		else if (can_execute(cmds[0]))
			ret = execve(cmds[0], cmds, g_shell.env);
		if (g_shell.path && g_shell.path[i])
			ret = execve(g_shell.path[i], cmds, g_shell.env);
		if (exec_error(ret, g_shell.path, cmds[0], g_shell.path[i]))
			exit_process(tab);
		_exit(ret);
	}
	free_cmd_lst(g_shell.path);
	free_cmd_lst(g_shell.env);
	return (1);
}
