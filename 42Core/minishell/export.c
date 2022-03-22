/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 15:20:40 by bmachado          #+#    #+#             */
/*   Updated: 2021/12/16 12:41:48 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_env_var(char *cmd, t_shell *table)
{
	int		i;
	char	*tmpkey;
	char	*tmpvalue;

	i = 0;
	tmpkey = NULL;
	tmpvalue = ft_strdup("");
	if (is_local_declare(cmd))
	{
		while (cmd[i] != '=')
			i++;
		tmpkey = ft_substr(cmd, 0, i);
		tmpvalue = ft_substr(cmd, i + 1, ft_strlen(cmd));
		table->env_print = 1;
	}
	else
	{
		tmpkey = cmd;
		table->env_print = 0;
	}
	ft_export_env(table, tmpkey, tmpvalue, table->env_print);
	return (1);
}

int	print_exported_variables(t_shell *table)
{
	char	**sorted_environ;
	int		i;

	i = -1;
	sorted_environ = (char **)ft_calloc(sizeof(char *), table->env_count + 1);
	if (!sorted_environ)
		return (0);
	sort_env(sorted_environ, table);
	while (sorted_environ[++i])
		printf("declare -x %s\n", sorted_environ[i]);
	free_cmd_lst(sorted_environ);
	return (1);
}

int	export_function(char **cmd, t_shell *table)
{
	int	i;

	i = 0;
	if (!cmd[1])
		return (print_exported_variables(table));
	while (cmd[++i])
		if (!create_env_var(cmd[i], table))
			return (0);
	return (1);
}

static char	*define_value(t_shell *table, char *key, char *value)
{
	int		i;
	char	*set_value;
	t_vars	*local;

	i = get_position(table, key);
	local = table->local[i];
	if (!local)
		return (value);
	set_value = local->value;
	return (set_value);
}

void	ft_export_env(t_shell *table, char *key, char *value, int set_print)
{
	t_vars	*current;
	char	*old_value;
	int		i;

	old_value = value;
	i = get_position_env(table, key);
	current = table->env_vars[i];
	if (current)
		hash_insert_env(table, key, value);
	else
	{
		current = table->env_vars[table->env_count];
		if (!current)
		{
			value = define_value(table, key, value);
			current = create_vars(key, value);
			if (!set_print && !ft_strcmp(value, old_value))
				current->print = 0;
			table->env_vars[table->env_count] = current;
			unset(key, table);
			table->env_count++;
		}
	}
}
