/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 11:52:22 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 15:03:43 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_initial_input(t_shell *s)
{
	s->proc.child = 0;
	s->proc.tmpin = dup(0);
	if (s->proc.tmpin < 0)
	{
		puts("tmpin ko");
		return ;
	}
	s->proc.tmpout = dup(1);
	if (s->proc.tmpout < 0)
	{
		puts("tmpout ko");
		return ;
	}
}

void	restore_initial_input(t_shell *s)
{
	if (dup2(s->proc.tmpin, 0) < 0
		|| dup2(s->proc.tmpout, 1) < 0)
	{
		puts("dup2 tmpin tmpout ko");
		return ;
	}
	close(s->proc.tmpin);
	close(s->proc.tmpout);
}

void	dup_close_fds(t_shell *s, int type)
{
	if (type == 0)
	{
		if (dup2(s->proc.fdin, 0) < 0)
		{
			puts("fdin dup error");
			return ;
		}
		close(s->proc.fdin);
	}
	else
	{
		if (dup2(s->proc.fdout, 1) < 0)
		{
			puts("fdout closed");
			return ;
		}
		close(s->proc.fdout);
	}
}

void	wait_for_child(int child_quantity)
{
	int			i;
	int			status;

	i = -1;
	while (++i < child_quantity)
	{
		wait(&status);
		if (WIFEXITED(status))
			g_shell.error = WEXITSTATUS(status);
	}
}

int	exec_pipe(t_shell *table, char **cmds)
{
	int	p[2];

	dup_close_fds(table, 0);
	if (pipe(p) < 0)
	{
		puts("create pipe error");
		return (0);
	}
	table->proc.fdout = p[1];
	table->proc.fdin = p[0];
	if (dup2(table->proc.fdout, 1) < 0)
	{
		puts("exec pipe error");
		return (0);
	}
	close(table->proc.fdout);
	execute_cmd(table, cmds);
	return (1);
}
