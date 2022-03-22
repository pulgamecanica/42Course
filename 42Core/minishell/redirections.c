/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 14:29:31 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 15:10:24 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pass_redir_to_token(int ret, t_token **token_lst)
{
	if (ret == D_LESS)
		add_token(token_lst, create_token("<<", ret));
	if (ret == D_GREAT)
		add_token(token_lst, create_token(">>", ret));
	if (ret == GREAT)
		add_token(token_lst, create_token(">", ret));
	if (ret == LESS)
		add_token(token_lst, create_token("<", ret));
	return (1);
}

static int	define_flags(int type)
{
	if (type == D_GREAT)
		return (O_WRONLY | O_CREAT | O_APPEND);
	else if (type == GREAT)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else if (type == LESS)
		return (O_WRONLY | O_CREAT);
	return (0);
}

static void	create_file(char *file, int type, t_shell *s)
{
	int	fd;
	int	open_flags;

	open_flags = define_flags(type);
	if (type == D_GREAT || type == GREAT)
	{
		fd = open(file, open_flags, 0777);
		if (fd < 0)
			return ;
		s->proc.fdout = fd;
	}
}

int	check_for_redir(char **cmds, int start, t_shell *table)
{
	int	type;
	int	i;
	int	end;

	i = start;
	end = 0;
	while (cmds[i])
	{
		type = is_redir(cmds[i], 0);
		if ((type == GREAT || type == D_GREAT) && cmds[i + 1])
		{
			create_file(cmds[i + 1], type, table);
			if (table->redir.no_redirect && !table->redir.in)
				end = i;
			table->redir.no_redirect = 0;
		}
		i++;
	}
	return (end);
}

char	**redirected(t_shell *table, char **cmds, int start, int end)
{
	char	**r_cmd;

	table->redir.no_redirect = 1;
	end = check_for_redir(cmds, start, table);
	if (table->redir.in)
		end = redir_in(cmds, start, end);
	if (table->redir.no_redirect)
		table->proc.fdout = dup(table->proc.tmpout);
	r_cmd = ft_subarray(cmds, start, end);
	return (r_cmd);
}
