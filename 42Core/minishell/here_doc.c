/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 16:17:52 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 15:23:47 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	signal_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		g_shell.error = 130;
		printf("\n");
		exit(130);
	}
}

static void	here_doc_input(char *limiter, int doc_fd)
{
	char	*doc_input;

	signal(SIGINT, signal_heredoc);
	while (1)
	{
		doc_input = readline("> ");
		if (!doc_input)
		{
			close(doc_fd);
			break ;
		}
		if (ft_strcmp(doc_input, limiter) == 0)
		{
			close(doc_fd);
			free(doc_input);
			break ;
		}
		else if (ft_strcmp(doc_input, limiter))
			ft_putendl_fd(doc_input, doc_fd);
		free(doc_input);
	}
	exit(0);
}

static void	clear_doc(int status)
{
	int	clear_fd;

	clear_fd = open(TMP_PATH, defdoc(CLEARDOC), 0600);
	close(clear_fd);
	g_shell.error = WEXITSTATUS(status);
}

static void	close_input_doc(int doc_fd, t_shell *tab)
{
	int	fd;

	fd = open(TMP_PATH, O_RDONLY);
	tab->proc.fdin = fd;
	dup2(doc_fd, 0);
	close(doc_fd);
}

void	here_doc(char *limiter, t_shell *tab)
{
	int			doc_fd;
	int			fdout_backup;
	int			pid;
	static int	status;

	doc_fd = open(TMP_PATH, defdoc(CREATDOC), 0600);
	if (doc_fd == -1)
		return ;
	fdout_backup = dup(1);
	tab->proc.fdout = dup(1);
	pid = fork();
	if (pid == -1)
		return ;
	else if (pid == 0)
		here_doc_input(limiter, doc_fd);
	wait(&status);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		clear_doc(status);
	close_input_doc(doc_fd, tab);
	dup2(fdout_backup, 1);
	close(fdout_backup);
}
