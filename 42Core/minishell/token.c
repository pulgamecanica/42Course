/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 11:24:49 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 11:28:42 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_old_input(char *input)
{
	if (!ft_isfilled(input))
		return ;
	free(input);
}

static void	clear_token_error(t_token **token_lst)
{
	if (!token_lst)
		return ;
	clear_token_lst(token_lst);
}

static int	define_iter(char *input, int i)
{
	int	iter;

	iter = 0;
	if (is_redir(input, i) == D_GREAT || is_redir(input, i) == D_LESS)
		iter += 1;
	if (is_quote(input[i]))
		iter += quoted_len(input, i);
	iter++;
	return (iter);
}

static void	recursion_token(char *input, char *tmp, t_token **token_lst, int i)
{
	i += define_iter(input, i);
	tmp = ft_substr(input, i, ft_strlen(input));
	input = tmp;
	tokenizer(token_lst, input);
	free(tmp);
}

void	tokenizer(t_token **token_lst, char *input)
{
	int		next;
	char	*tmp;
	int		ret;
	int		i;

	i = -1;
	tmp = NULL;
	while (++i < (int)ft_strlen(input))
	{
		next = peek_next_one(input, i);
		ret = split_type(next, input, i, token_lst);
		if (ret > 0)
		{
			recursion_token(input, tmp, token_lst, i);
			break ;
		}
		else if (ret < 0)
		{
			clear_token_error(token_lst);
			break ;
		}
	}
	free_old_input(tmp);
}
