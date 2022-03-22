/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:47:35 by bmachado          #+#    #+#             */
/*   Updated: 2021/12/14 17:05:13 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_empty_slots(t_shell *tab, int slot)
{
	int		save;
	t_vars	*tmp;

	save = 0;
	if (tab->env_vars[slot + 1])
		save = slot + 1;
	else
		return ;
	tab->env_vars[slot] = tab->env_vars[save];
	while (tab->env_vars[slot + 1])
	{
		tmp = tab->env_vars[slot];
		tab->env_vars[slot] = tab->env_vars[slot + 1];
		if (!tab->env_vars[slot + 2])
			tmp = NULL;
		tab->env_vars[slot + 1] = tmp;
		slot++;
	}
}

static void	unset_env(char *key, t_shell *table)
{
	t_vars	*current;
	int		i;

	i = get_position_env(table, key);
	current = table->env_vars[i];
	if (!current)
		return ;
	else if (ft_strcmp(current->key, key) == 0)
	{
		free_vars(current);
		table->env_vars[i] = NULL;
		fill_empty_slots(table, i);
		table->env_count--;
	}
}

void	unset(char *key, t_shell *table)
{
	t_vars	*current;
	int		i;

	i = get_position(table, key);
	current = table->local[i];
	if (!current)
		return ;
	else if (ft_strcmp(current->key, key) == 0)
	{
		free_vars(current);
		table->local[i] = NULL;
		current = NULL;
	}
}

void	unset_function(t_shell *table, char **cmds)
{
	int	i;

	i = 1;
	while (cmds[i])
	{
		if (is_in_env(cmds[i], table))
			unset_env(cmds[i], table);
		else
			unset(cmds[i], table);
		i++;
	}
}
