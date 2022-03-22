/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 16:14:15 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 11:52:05 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_args(char **input, char *join)
{
	char	*tmp;

	tmp = ft_strjoin(*input, join);
	*input = tmp;
}

void	free_cmd_lst(char **cmd_lst)
{
	int	i;

	i = 0;
	if (!cmd_lst)
		return ;
	while (cmd_lst[i])
	{
		free(cmd_lst[i]);
		i++;
	}
	free(cmd_lst);
}

int	free_tokenlst(t_token **token_lst)
{
	clear_token_lst(token_lst);
	return (1);
}

void	free_vars(t_vars *var)
{
	free(var->key);
	free(var->value);
	free(var);
}

void	free_table(t_shell *table)
{
	int				i;
	t_vars			*current;

	i = 0;
	while (i < table->size)
	{
		current = table->local[i];
		if (current)
			free_vars(current);
		current = table->env_vars[i];
		if (current)
			free_vars(table->env_vars[i]);
		i++;
	}
	free(table->local);
	free(table->env_vars);
	free(table);
}
