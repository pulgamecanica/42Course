/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:43:41 by bmachado          #+#    #+#             */
/*   Updated: 2021/12/14 18:31:19 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_in(char **cmds, int start, int end)
{
	int	set;

	if (!cmds[start])
		return (end);
	while (cmds[start])
	{
		set = is_redir(cmds[start], 0);
		if (set)
			break ;
		end++;
		start++;
	}
	return (end);
}

int	search_in_redirections(char **cmds, t_shell *tab)
{
	int	i;
	int	type;

	i = 0;
	tab->redir.in = 0;
	if (!is_redir(cmds[0], 0))
		return (redir_in_backwards(cmds, tab));
	while (cmds[i])
	{
		type = is_redir(cmds[i], 0);
		if ((type == LESS || type == D_LESS) && !tab->redir.open_error)
			open_file(cmds[i + 1], type, tab);
		if ((!type && cmds[i + 1] && !is_redir(cmds[i + 1], 0)) || !cmds[i + 1])
			break ;
		i++;
	}
	if (tab->redir.doc_flag ||!tab->redir.no_redirect)
		i += 1;
	return (i);
}

int	redir_status(int i, t_shell *status)
{
	if (status->redir.open_error)
		return (-1);
	return (i);
}

int	redir_in_backwards(char **cmds, t_shell *tab)
{
	int	i;
	int	type;

	if (tab->redir.open_error)
		return (-1);
	i = ft_arrlen(cmds) - 1;
	while (i >= 0)
	{
		type = is_redir(cmds[i], 0);
		if ((type == LESS || type == D_LESS) && !tab->redir.in)
		{
			open_file(cmds[i + 1], type, tab);
			tab->redir.in = 1;
		}
		i--;
	}
	return (i + 1);
}
