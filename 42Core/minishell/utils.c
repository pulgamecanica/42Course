/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 15:54:29 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 15:43:22 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_whitespace(int c)
{
	if (c == '\t' || c == '\n' || c == '\r'
		|| c == '\v' || c == '\f' || c == ' ')
		return (1);
	return (0);
}

int	get_input(t_shell *shell, char **buff)
{
	char	*tbuff;

	input_signal();
	*buff = readline("Minishell %% ");
	if (*buff)
	{
		tbuff = ft_strtrim(*buff, " ");
		ft_strcpy(*buff, tbuff);
		if (ft_isfilled(tbuff))
			add_history(tbuff);
		free(tbuff);
		return (1);
	}
	else
	{
		printf("exit\n");
		shell->running = 0;
	}
	return (0);
}

void	send_to_cmd(char **cmds, int i, char *joined_cmd, t_token *tk)
{
	if (joined_cmd)
		cmds[i] = joined_cmd;
	else if (tk->value)
		cmds[i] = tk->value;
}

int	is_number(char *num)
{
	int	i;

	i = 0;
	while (num[i])
	{
		if (!ft_isdigit(num[i]) && num[i] != '-' && num[i] != '+')
			return (0);
		i++;
	}
	return (1);
}

int	valid_quote(char *input)
{
	int	dq;
	int	q;
	int	count;
	int	i;

	i = -1;
	dq = 0;
	q = 0;
	count = 0;
	while (input[++i])
	{
		if (input[i] == '\'' && !q && !dq)
			q = 1;
		else if (input[i] == '\"' && !q && !dq)
			dq = 1;
		else if (!dq && q && input[i] == '\'')
			q = 0;
		else if (!q && dq && input[i] == '\"')
			dq = 0;
		else
			continue ;
		count++;
	}
	return (count % 2 == 0);
}
