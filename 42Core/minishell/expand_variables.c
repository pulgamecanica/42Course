/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 17:54:50 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 10:48:52 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_vars(t_token **token_lst, t_shell *table)
{
	t_token	*token;
	char	*tmp;

	token = *token_lst;
	while (token)
	{
		if (token->type == DOLLAR)
		{
			if (ft_strcmp(token->value, "$?") == 0)
			{
				tmp = ft_itoa(g_shell.error);
				token->value = tmp;
			}
			else
			{
				tmp = expanded_var(token->value, table);
				token->value = tmp;
			}
		}
		token = token->next;
	}
}
