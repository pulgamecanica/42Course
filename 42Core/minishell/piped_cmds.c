/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piped_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 16:22:52 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 15:24:30 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**slice_less(char **pcmds)
{
	int	i;

	i = 0;
	while (pcmds[i])
	{
		if (is_redir(pcmds[i], 0) && pcmds[i + 1])
		{
			pcmds[i] = pcmds[i + 1];
			pcmds[i + 1] = NULL;
		}
		i++;
	}
	return (pcmds);
}

static void	checkdoc(t_shell *tab, char **pcmds)
{
	int	i;

	i = ft_arrlen(pcmds) - 1;
	if (tab->redir.doc_flag)
		pcmds[i] = TMP_PATH;
	else
		return ;
}

int	piped_cmd(t_shell *table, char **cmds, int start, int end)
{
	char	**pcmds;

	pcmds = ft_subarray(cmds, start, end);
	if (table->redir.in)
	{
		pcmds = slice_less(pcmds);
		checkdoc(table, pcmds);
	}
	exec_pipe(table, pcmds);
	free(pcmds);
	return (end + 1);
}

void	last_piped_cmd(t_shell *tab, char **cmds, int start, int end)
{
	char	**pcmds;

	dup_close_fds(tab, 0);
	pcmds = redirected(tab, cmds, start, end);
	dup_close_fds(tab, 1);
	execute_cmd(tab, pcmds);
	free(pcmds);
}
