/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 11:23:33 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 11:57:26 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(char *value, int type)
{
	t_token	*token;

	token = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

t_token	*find_last_token(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}

int	token_size(t_token *token_lst)
{
	t_token	*token;
	int		i;

	i = 0;
	if (!token_lst)
		return (0);
	token = token_lst;
	while (token)
	{
		if (token->type != SPACE)
			i++;
		token = token->next;
	}
	return (i);
}

void	add_token(t_token **token_lst, t_token *new_token)
{
	t_token	*token;

	if (!token_lst || !new_token)
		return ;
	if (!*token_lst)
	{
		*token_lst = new_token;
		return ;
	}
	token = find_last_token(*token_lst);
	token->next = new_token;
	token->prev = *token_lst;
}

void	clear_token_lst(t_token **token_lst)
{
	t_token	*token;
	t_token	*tmp;

	if (!token_lst)
		return ;
	token = *token_lst;
	while (token)
	{
		tmp = token->next;
		if (!is_operator(token->type) && token->type != DOLLAR)
			free(token->value);
		free(token);
		token = tmp;
	}
	*token_lst = NULL;
}
