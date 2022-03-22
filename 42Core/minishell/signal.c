/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 13:30:33 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 14:08:26 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	interrupt(int signal)
{
	if (signal == SIGINT)
	{
		g_shell.error = 130;
		printf("^C\n");
	}
}

void	quit(int signal)
{
	if (signal == SIGQUIT)
	{
		g_shell.error = 131;
		printf("minishell: quit process\n");
	}
}

void	redisplay(int signal)
{
	if (signal == SIGINT)
	{
		g_shell.error = 130;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	exec_signal(void)
{
	signal(SIGINT, interrupt);
	signal(SIGQUIT, quit);
}

void	input_signal(void)
{
	signal(SIGINT, redisplay);
	signal(SIGQUIT, SIG_IGN);
}
