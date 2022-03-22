/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 11:27:10 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 11:13:05 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	define_type(char c)
{
	if (c == '$')
		return (DOLLAR);
	else if (c == '|')
		return (PIPE);
	else if (c == '\"')
		return (D_QUOTES);
	else if (c == '\'')
		return (QUOTES);
	else if (c == ' ')
		return (SPACE);
	else if (c == '=')
		return (EQUAL);
	else
		return (CHAR);
}

int	not_var_name(char *input, int i)
{
	while (i >= 0)
	{
		if (define_type(input[i]) == DOLLAR)
			return (0);
		i--;
	}
	return (1);
}

int	peek_next_one(char *input, int i)
{
	return (define_type(input[i + 1]));
}

int	only_char_before(char *input, int i)
{
	while (i > 0)
	{
		if (define_type(input[i - 1]) != CHAR)
			return (0);
		i--;
	}
	return (1);
}
