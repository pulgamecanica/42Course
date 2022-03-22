/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 12:11:40 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 10:49:24 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_key(char *key)
{
	int		i;
	char	*tmp;

	i = 0;
	while (key[i])
	{
		if (key[i] == '$')
			break ;
		i++;
	}
	tmp = ft_substr(key, i + 1, ft_strlen(key));
	return (tmp);
}

int	local_exists(char *key, t_shell *table)
{
	int		i;
	t_vars	*current;

	i = get_position(table, key);
	current = table->local[i];
	if (!current && is_in_env(key, table))
		return (ENV);
	else if (!current && !is_in_env(key, table))
		return (0);
	return (LOCAL);
}

int	is_var_call(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		if (key[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

char	*expanded_var(char *key, t_shell *table)
{
	int		i;
	int		check;
	t_vars	*current;

	if (ft_strcmp(key, "$") == 0)
		return (key);
	key = get_key(key);
	check = local_exists(key, table);
	current = NULL;
	if (check == 0)
		return (NULL);
	else if (check == LOCAL)
	{
		i = get_position(table, key);
		current = table->local[i];
	}
	else if (check == ENV)
	{
		i = get_position_env(table, key);
		current = table->env_vars[i];
	}
	free(key);
	return (current->value);
}
