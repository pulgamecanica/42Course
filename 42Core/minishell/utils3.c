/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 16:53:45 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 10:54:18 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	split_redir(char *input, int i, t_token **token_lst)
{
	int	ret;

	if (i - 1 >= 0)
		pass_to_token(input, i - 1, token_lst);
	ret = is_redir(input, i);
	pass_redir_to_token(ret, token_lst);
	return (1);
}

static int	split_arg(char *input, int i, t_token **token_lst)
{
	if (define_type(input[i]) == PIPE || define_type(input[i]) == SPACE)
		return (0);
	pass_to_token(input, i, token_lst);
	return (1);
}

static int	split_var(char *input, int i, t_token **token_lst)
{
	if (define_type(input[i]) == PIPE)
		i++;
	if (input[0] != '$')
		pass_to_token(input, i - 1, token_lst);
	var_to_token(input, i, token_lst);
	return (1);
}

static int	split_pipe(char *input, int i, t_token **token_lst)
{
	if (peek_next_one(input, i + 1) == PIPE)
	{
		printf("minishell: parse error near \'|\'\n");
		g_shell.error = 1;
		return (-1);
	}
	pass_to_token(input, i, token_lst);
	add_token(token_lst, create_token("|", PIPE));
	return (1);
}

int	split_type(int next, char *input, int i, t_token **token_lst)
{
	int	actual;

	actual = define_type(input[i]);
	if (actual == DOLLAR && (next == CHAR))
		split_var(input, i, token_lst);
	else if (actual == EQUAL)
		return (split_equal(input, i, token_lst));
	else if ((actual == D_QUOTES || actual == QUOTES))
		return (parse_quote(input, i, token_lst));
	else if (actual != next && next == PIPE)
		return (split_pipe(input, i, token_lst));
	else if (is_redir(input, i))
		return (split_redir(input, i, token_lst));
	else if ((actual != next && next == SPACE)
		|| (not_var_name(input, i) == 1 && !input[i + 1]))
		return (split_arg(input, i, token_lst));
	else if (actual == SPACE && next != SPACE)
	{
		add_token(token_lst, create_token(" ", SPACE));
		return (1);
	}
	return (0);
}
