/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 15:53:18 by bmachado          #+#    #+#             */
/*   Updated: 2021/12/13 12:55:24 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_in_env(char *key, t_shell *table)
{
	int		i;
	t_vars	*current;

	i = get_position_env(table, key);
	current = table->env_vars[i];
	if (current)
		return (1);
	return (0);
}
