/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 11:17:19 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 15:43:36 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fix_join(int type, int next_type)
{
	if (type == PIPE || type == SPACE || redir_type(type))
		return (0);
	else if (next_type == PIPE || next_type == SPACE || redir_type(next_type))
		return (0);
	return (1);
}

char	**create_cmd_lst(t_token **token_lst, char **cmd, int i)
{
	char	*tmp;
	t_token	*token;

	token = *token_lst;
	tmp = token->value;
	while (token)
	{
		if (token->next && fix_join(token->type, token->next->type))
			join_args(&tmp, token->next->value);
		else if ((token->next && is_operator(token->type)) || !token->next)
		{
			send_to_cmd(cmd, i++, tmp, token);
			i += get_operator(token, cmd, i);
			while (token->next && is_operator(token->next->type))
			{
				if (check_operator(token))
					cmd[i++] = token->next->value;
				token = token->next;
			}
			if (token->next)
				tmp = token->next->value;
		}
		token = token->next;
	}
	return (cmd);
}

static int	pipe_issue(char *input)
{
	int	i;

	i = 0;
	if (input[i] == '|')
	{
		printf("minishell: parse error near \'|\'\n");
		g_shell.error = 130;
		return (1);
	}
	while (input[i])
	{
		if (input[i] == '|' && !input[i + 1])
		{
			printf("minishell: parse error near \'|\'\n");
			g_shell.error = 130;
			return (1);
		}
		i++;
	}
	return (0);
}

static int	invalid_redir(t_token **token_lst)
{
	t_token	*tk;

	tk = *token_lst;
	while (tk)
	{
		if (redir_type(tk->type) && (!tk->next || redir_type(tk->next->type)))
		{
			printf("minishell: parse error near \'\\n\'\n");
			g_shell.error = 130;
			return (1);
		}
		else if (is_operator(tk->type) && tk->next && tk->next->type == EQUAL)
		{
			if (tk->next->next)
				printf("minishell: not found: %s\n", tk->next->next->value);
			g_shell.error = 1;
			return (1);
		}
		tk = tk->next;
	}	
	return (0);
}

int	set_cmd_lst(t_shell *table, t_token *token_lst, char *input)
{
	char	**cmd_lst;
	int		i;

	i = 0;
	if (pipe_issue(input))
		return (0);
	tokenizer(&token_lst, input);
	if (token_size(token_lst) == 0 || invalid_redir(&token_lst)
		|| invalid_assig(&token_lst, table))
		return (free_tokenlst(&token_lst));
	expand_vars(&token_lst, table);
	cmd_lst = (char **)calloc(sizeof(char *), token_size(token_lst) + 1);
	if (!cmd_lst)
		return (0);
	cmd_lst = create_cmd_lst(&token_lst, cmd_lst, i);
	cmd_lst[token_size(token_lst)] = NULL;
	if (!cmd_lst)
		return (free_tokenlst(&token_lst));
	exec_parsed_cmdlst(table, cmd_lst, &token_lst);
	free(cmd_lst);
	clear_token_lst(&token_lst);
	return (1);
}
