/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 16:10:07 by bmachado          #+#    #+#             */
/*   Updated: 2021/12/15 15:54:46 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_type(int type)
{
	if (type == D_LESS || type == D_GREAT
		|| type == GREAT || type == LESS)
		return (1);
	return (0);
}

int	is_redir(char *input, int i)
{
	if (!ft_isfilled(input))
		return (0);
	if (input[i + 1] && input[i] == '<' && input[i + 1] == '<')
		return (D_LESS);
	else if (input[i + 1] && input[i] == '>' && input[i + 1] == '>')
		return (D_GREAT);
	else if (input[i] == '>')
		return (GREAT);
	else if (input[i] == '<')
		return (LESS);
	return (0);
}

void	open_file(char *file, int type, t_shell *s)
{
	int	fd;

	if (type == LESS)
	{
		fd = open(file, O_RDONLY, 0777);
		if (fd < 0)
		{
			printf("minishell: No such file or directory: %s\n", file);
			g_shell.error = 1;
			s->redir.open_error = 1;
		}
		s->redir.no_redirect = 0;
		s->proc.fdin = fd;
	}
	else if (type == D_LESS)
	{
		here_doc(file, s);
		s->redir.doc_flag = 1;
		s->redir.no_redirect = 0;
	}
}

static void	redirect_flags(t_shell *table)
{
	table->redir.no_redirect = 1;
	table->redir.doc_flag = 0;
	table->redir.open_error = 0;
}

int	in_redirected(char **cmds, t_shell *table)
{
	int	cmd_start;

	redirect_flags(table);
	cmd_start = search_in_redirections(cmds, table);
	if (table->redir.no_redirect)
		table->proc.fdin = dup(table->proc.tmpin);
	return (redir_status(cmd_start, table));
}
