/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_splits.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 11:44:57 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 10:52:38 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(int token_type)
{
	if (token_type == PIPE || token_type == SPACE
		|| redir_type(token_type) || token_type == EQUAL)
		return (1);
	return (0);
}

int	is_empty_var(t_shell *table, char *str)
{
	if (str[0] == '$' && !expanded_var(str, table))
		return (1);
	return (0);
}

int	check_operator(t_token *tk)
{
	if (tk->next->type == PIPE || redir_type(tk->next->type))
		return (1);
	return (0);
}

int	send_operator(t_token *tk)
{
	if (!tk->next || !tk->prev)
		return (0);
	if (!is_operator(tk->next->type)
		&& (tk->type == PIPE || redir_type(tk->type)))
		return (1);
	if (!is_operator(tk->prev->type) && is_operator(tk->next->type)
		&& (tk->type == PIPE || redir_type(tk->type)))
		return (1);
	return (0);
}

int	get_operator(t_token *token, char **cmd, int i)
{
	if (send_operator(token))
	{
		cmd[i] = ft_strdup(token->value);
		return (1);
	}
	return (0);
}
