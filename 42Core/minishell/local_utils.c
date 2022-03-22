/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 15:42:41 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 12:07:20 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_vars	*create_vars(char *key, char *value)
{
	t_vars	*var;

	var = (t_vars *)ft_calloc(sizeof(t_vars), 1);
	if (!var)
		return (NULL);
	var->key = ft_strdup(key);
	var->value = ft_strdup(value);
	var->print = 1;
	if (!var->key || !var->value)
		return (NULL);
	return (var);
}

t_shell	*create_table(int size)
{
	t_shell	*table;
	int		i;

	i = -1;
	table = (t_shell *)ft_calloc(sizeof(t_shell), 1);
	table->size = size;
	table->n_locals = 0;
	table->local = (t_vars **)ft_calloc(sizeof(t_vars *), size);
	table->env_vars = (t_vars **)ft_calloc(sizeof(t_vars *), size);
	if (!table || !table->local || !table->env_vars)
		return (NULL);
	while (++i < size)
	{
		table->env_vars[i] = NULL;
		table->local[i] = NULL;
	}
	return (table);
}
