/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 16:28:28 by bmachado          #+#    #+#             */
/*   Updated: 2021/12/14 17:06:56 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_2d_array(char **array, int size)
{
	char	*tmp;
	int		i;
	int		j;
	int		j_min;

	i = -1;
	while (++i < size - 1)
	{
		j_min = i;
		j = i + 1;
		while (++j < size)
		{
			if (array[j_min] && array[j]
				&& ft_strcmp(array[j], array[j_min]) < 0)
				j_min = j;
		}
		if (j_min != i)
		{
			tmp = array[i];
			array[i] = array[j_min];
			array[j_min] = tmp;
		}
	}
}

void	sort_env(char **sorted, t_shell *table)
{
	int		i;
	char	*tmp;
	t_vars	*current;

	i = -1;
	while (++i < table->env_count)
	{
		current = table->env_vars[i];
		if (current)
		{
			tmp = ft_strjoin(current->key, "=");
			tmp = ft_strjoin(tmp, "\"");
			tmp = ft_strjoin(tmp, current->value);
			sorted[i] = ft_strjoin(tmp, "\"");
		}
	}
	sort_2d_array(sorted, table->env_count);
}
