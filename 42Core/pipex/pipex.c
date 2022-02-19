/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 17:09:55 by arosado-          #+#    #+#             */
/*   Updated: 2021/12/21 17:09:58 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	puterror(char *str)
{
	ft_putstr_fd("\033[0;31m", 2);
	perror(str);
	ft_putstr_fd("\033[0m", 2);
	exit(-1);
}

static char	*find_path(char **envp, char *cmd)
{
	char	**paths;
	char	*cmd_path;
	char	*sub;

	cmd = *ft_split(cmd, ' ');
	while (*envp && !ft_strnstr(*envp, "PATH=", 5))
		envp++;
	sub = ft_substr(*envp, 5, ft_strlen(*envp) - 5);
	paths = ft_split(sub, ':');
	free(sub);
	sub = ft_strjoin("/", cmd);
	while (*paths)
	{
		cmd_path = ft_strjoin(*paths, sub);
		if (!cmd_path)
			return (NULL);
		if (!access(cmd_path, F_OK))
			break ;
		free(cmd_path);
		paths++;
	}
	free(sub);
	free(cmd);
	return (cmd_path);
}

static void	out_process(int fds[2], char *outfile, char *cmd, char **envp)
{
	int		fd;
	int		execstat;
	int		pid;
	char	*path;

	pid = fork();
	if (pid == -1)
		puterror("Fork Error ");
	else if (pid == 0)
	{
		close(fds[1]);
		fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fd == -1)
			puterror("Infile Error ");
		dup2(fds[0], STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		path = find_path(envp, cmd);
		if (!path)
			puterror("Command not found ");
		execstat = execve(path, ft_split(cmd, ' '), envp);
		if (execstat == -1)
			puterror("Execve Error command failed/not founded ");
	}
}

static void	in_process(int fds[2], char *infile, char *cmd, char **envp)
{
	int		fd;
	int		execstat;
	int		pid;
	char	*path;

	pid = fork();
	if (pid == -1)
		puterror("In process, Fork Error ");
	else if (pid == 0)
	{
		close(fds[0]);
		fd = open(infile, O_RDONLY);
		if (fd == -1)
			puterror("Infile Error ");
		dup2(fds[1], STDOUT_FILENO);
		dup2(fd, STDIN_FILENO);
		path = find_path(envp, cmd);
		if (!path)
			puterror("Command not found ");
		execstat = execve(path, ft_split(cmd, ' '), envp);
		if (execstat == -1)
			puterror("Execve Error command failed/not founded ");
	}
}

int	main(int ac, char **av, char **envp)
{
	int	fds[2];

	if (ac != 5)
	{
		ft_putstr_fd("Usage: ./pipex filein cmd1 cmd2 fileout\n", 2);
		return (0);
	}
	if (pipe(fds) == -1)
		puterror("Pipe Error ");
	in_process(fds, av[1], av[2], envp);
	wait(NULL);
	out_process(fds, av[4], av[3], envp);
	close(fds[0]);
	close(fds[1]);
	return (0);
}
