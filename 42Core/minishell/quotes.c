/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 15:55:27 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 11:12:51 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_quoted(char *input, t_token **token_lst)
{
	if (no_dollarsign(input))
		add_token(token_lst, create_token(input, CHAR));
	else
		search_for_variable(input, token_lst);
}

int	add_quoted_var(char *input, int i, t_token **token_lst)
{
	char	*tmp;
	int		p;

	p = i;
	while (input[p])
	{
		if (define_type(input[p + 1]) != CHAR)
			break ;
		p++;
	}
	tmp = ft_substr(input, i, p - i + 1);
	if (ft_isfilled(tmp))
		add_token(token_lst, create_token(tmp, DOLLAR));
	return (p);
}

void	search_for_variable(char *str, t_token **token_lst)
{
	int		i;
	int		dollar;
	char	*tmp;
	int		start;

	i = 0;
	dollar = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (!(str[start] == '$' && dollar == 1))
			{
				tmp = ft_substr(str, start, i);
				add_token(token_lst, create_token(tmp, CHAR));
			}
			add_quoted_var(str, i, token_lst);
			add_after(str, i, token_lst);
			start = i;
			dollar = 1;
		}
		i++;
	}
}

int	split_doublequote(char *input, int i, t_token **token_lst)
{
	char		*tmp;
	int			init_type;
	int			end_type;
	int			p;

	init_type = define_type(input[i]);
	end_type = 0;
	i += 1;
	p = i;
	while (input[p])
	{
		end_type = define_type(input[p]);
		if (init_type == end_type)
			break ;
		p++;
	}
	tmp = ft_substr(input, i, p - i);
	add_quoted(tmp, token_lst);
	return (1);
}

int	split_singlequote(char *input, int i, t_token **token_lst)
{
	char	*tmp;
	int		init_type;
	int		end_type;
	int		p;

	init_type = define_type(input[i]);
	end_type = 0;
	i += 1;
	p = i;
	while (input[p])
	{
		end_type = define_type(input[p]);
		if (init_type == end_type)
			break ;
		p++;
	}
	tmp = ft_substr(input, i, p - i);
	add_token(token_lst, create_token(tmp, QUOTES));
	return (1);
}
