/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 16:41:28 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 15:44:53 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	no_dollarsign(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (0);
		i++;
	}
	return (1);
}

void	add_after(char *input, int i, t_token **token_lst)
{
	int		start;
	char	*tmp;

	i += 1;
	while (input[i])
	{
		if (define_type(input[i]) != 1)
			break ;
		i++;
	}
	start = i;
	while (input[i])
	{
		if (input[i] == '$' || define_type(input[i]) == D_QUOTES)
			break ;
		i++;
	}
	tmp = ft_substr(input, start, i - start);
	if (ft_isfilled(tmp))
		add_token(token_lst, create_token(tmp, CHAR));
}

int	parse_quote(char *input, int i, t_token **token_lst)
{
	int	type;

	type = define_type(input[i]);
	pass_to_token(input, i - 1, token_lst);
	if (type == D_QUOTES)
		return (split_doublequote(input, i, token_lst));
	else if (type == QUOTES)
		return (split_singlequote(input, i, token_lst));
	return (1);
}

int	is_quote(int input)
{
	int	type;

	type = define_type(input);
	if (type == D_QUOTES || type == QUOTES)
		return (1);
	return (0);
}

int	quoted_len(char *input, int i)
{
	int	init_type;
	int	end_type;
	int	p;

	init_type = define_type(input[i]);
	p = i;
	p += 1;
	while (input[p])
	{
		end_type = define_type(input[p]);
		if (init_type == end_type)
			return (p - i);
		p++;
	}
	return (0);
}
