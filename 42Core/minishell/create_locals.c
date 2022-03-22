/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_locals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 15:44:04 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 13:06:34 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_var_repeated(t_shell *table, char *key)
{
	int		i;
	t_vars	*current;

	i = 0;
	while (i < table->size)
	{
		current = table->local[i];
		if (current && ft_strcmp(current->key, key) == 0)
			return (i);
		i++;
	}
	return (0);
}

int	is_local_declare(char *str)
{
	int	i;
	int	has_equal;

	i = 0;
	has_equal = 0;
	if ((ft_strcmp(str, "=") == 0) || str[0] == '=')
		return (-1);
	while (str[i])
	{
		if (is_whitespace(str[i]))
			return (0);
		if (str[i] == '=')
			has_equal = 1;
		i++;
	}
	if (has_equal == 0)
		return (0);
	return (1);
}

int	get_position(t_shell *table, char *key)
{
	int		i;
	int		check_var;
	t_vars	*current;

	i = 0;
	check_var = is_var_repeated(table, key);
	if (check_var != 0)
		return (check_var);
	while (i < table->n_locals)
	{
		current = table->local[i];
		if (current && ft_strcmp(current->key, key) == 0)
			break ;
		if (!current)
			break ;
		i++;
	}
	return (i);
}

void	hash_insert(t_shell *table, char *key, char *value)
{
	int		i;
	t_vars	*current_var;

	i = get_position(table, key);
	current_var = table->local[i];
	if (current_var)
	{
		if (ft_strcmp(current_var->key, key) == 0)
		{
			free(current_var->value);
			current_var->value = ft_strdup(value);
			ft_strcpy(table->local[i]->value, value);
			return ;
		}
	}
	else if (!current_var)
	{
		current_var = create_vars(key, value);
		table->local[i] = current_var;
		table->n_locals++;
	}
}

int	create_locals(char *var, t_shell *table)
{
	int		i;
	char	*tmpkey;
	char	*tmpvalue;

	i = 0;
	while (var[i] != '=')
		i++;
	tmpkey = ft_substr(var, 0, i);
	tmpvalue = ft_substr(var, i + 1, ft_strlen(var));
	if (is_local_declare(var) && is_in_env(tmpkey, table))
		hash_insert_env(table, tmpkey, tmpvalue);
	else
		hash_insert(table, tmpkey, tmpvalue);
	free(tmpkey);
	free(tmpvalue);
	return (1);
}
