/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 16:15:59 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 11:25:33 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pass_to_token(char *input, int i, t_token **token_lst)
{
	int		start;
	int		type;
	char	*tmp;

	if (i < 0)
		return ;
	start = i;
	if (input[i] == '$' && define_type(input[i + 1]) != QUOTES)
	{
		add_token(token_lst, create_token(ft_strdup("$"), CHAR));
		return ;
	}
	if (!not_var_name(input, i))
		return ;
	while (start > 0)
	{
		type = define_type(input[start]);
		if (type != define_type(input[start - 1]))
			break ;
		start--;
	}
	tmp = ft_substr(input, start, i - start + 1);
	add_token(token_lst, create_token(tmp, define_type(input[start])));
}

void	var_to_token(char *input, int i, t_token **token_lst)
{
	int		end;
	char	*tmp;

	end = i + 1;
	while (input[end])
	{
		if (define_type(input[end]) != CHAR)
			break ;
		end++;
	}
	tmp = ft_substr(input, i, end - i);
	add_token(token_lst, create_token(tmp, DOLLAR));
}
