/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 18:23:14 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/18 15:19:45 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell_env(t_shell *table)
{
	int		i;
	char	*tmp;
	t_vars	*current;

	i = -1;
	g_shell.env = (char **)ft_calloc(sizeof(char *), table->env_count + 1);
	if (!g_shell.env)
		return ;
	while (++i < table->env_count)
	{
		current = table->env_vars[i];
		if (current)
		{
			tmp = ft_strjoin(current->key, "=");
			g_shell.env[i] = ft_strjoin(tmp, current->value);
			free(tmp);
		}
	}
	g_shell.env[table->env_count] = NULL;
}
