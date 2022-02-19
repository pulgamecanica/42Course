/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 17:09:55 by arosado-          #+#    #+#             */
/*   Updated: 2022/01/05 01:53:44 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	out_process(char *outfile, char *cmd, char **envp)
{
	int		fd;
	int		execstat;
	char	*path;

	fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
		puterror("Infile Error ");
	dup2(fd, STDOUT_FILENO);
	path = find_path(envp, cmd);
	if (!path)
		puterror("Command not found ");
	execstat = execve(path, ft_split(cmd, ' '), envp);
	if (execstat == -1)
		puterror("Execve Error command failed/not founded ");
}

static void	in_process(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd == -1)
		puterror("Infile Error ");
	dup2(fd, STDIN_FILENO);
}

static void	do_pipe(char *cmd, char **envp)
{
	int		fds[2];
	int		pid;
	char	*path;

	if (pipe(fds) == -1)
		puterror("Pipe Error ");
	pid = fork();
	if (pid == -1)
		puterror("Fork Error ");
	if (pid == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		path = find_path(envp, cmd);
		if (!path || execve(path, ft_split(cmd, ' '), envp) == -1)
			puterror("Execve Error command failed/not founded ");
	}
	else
	{
		close(fds[1]);
		wait(NULL);
		dup2(fds[0], STDIN_FILENO);
	}
}

void	listen(char *limiter)
{
	char	*line;
	int		fds[2];
	int		pid;

	if (pipe(fds) == -1)
		puterror("Pipe Error ");
	pid = fork();
	if (pid == 0)
	{
		while (1)
		{
			line = get_next_line(0);
			if (ft_strnstr(line, limiter, ft_strlen(limiter)))
				exit(-1);
			write(fds[1], line, ft_strlen(line));
		}
	}
	else
	{
		dup2(fds[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
	close(fds[0]);
	close(fds[1]);
}

int	main(int ac, char **av, char **envp)
{
	int		index;

	if (ac < 5)
	{
		ft_putstr_fd("Usage: ./pipex filein cmd1 cmd2 cmdN ... fileout\n", 2);
		ft_putstr_fd("Usage: ./pipex here_doc LIMITER cmdN ... fileout\n", 2);
		return (0);
	}
	index = 2;
	if (!ft_strncmp(av[1], "here_doc", 8))
	{
		listen(av[2]);
		index = 3;
	}
	else
		in_process(av[1]);
	while (index < ac - 2)
		do_pipe(av[index++], envp);
	out_process(av[ac - 1], av[ac - 2], envp);
	return (0);
}
