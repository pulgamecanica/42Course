/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 15:56:13 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 13:00:10 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_position_env(t_shell *table, char *key)
{
	int		i;
	int		check_var;
	t_vars	*current;

	i = 0;
	check_var = is_var_repeated(table, key);
	if (check_var != 0)
		return (check_var);
	while (i < table->size)
	{
		current = table->env_vars[i];
		if (current && ft_strcmp(current->key, key) == 0)
			break ;
		if (!current)
			break ;
		i++;
	}
	return (i);
}

void	hash_insert_env(t_shell *table, char *key, char *value)
{
	int		i;
	t_vars	*current_item;

	i = get_position_env(table, key);
	current_item = table->env_vars[i];
	if (current_item)
	{	
		if (ft_strcmp(current_item->key, key) == 0)
		{
			free(current_item->value);
			current_item->value = ft_strdup(value);
			return ;
		}
	}
	if (!current_item)
	{
		current_item = create_vars(key, value);
		table->env_vars[i] = current_item;
		table->env_count++;
	}
}

void	envtable(t_shell *table, char **env)
{
	int		i;
	int		p;
	char	*tmpkey;
	char	*tmpvalue;

	i = -1;
	table->env_count = 0;
	while (++i < ft_arrlen(env))
	{
		p = 0;
		while (env[i][p])
		{
			if (env[i][p] == '=')
			{
				tmpkey = ft_substr(env[i], 0, p);
				tmpvalue = ft_substr(env[i], p + 1, ft_strlen(env[i]));
				hash_insert_env(table, tmpkey, tmpvalue);
				free(tmpkey);
				free(tmpvalue);
			}
			p++;
		}
	}
}
