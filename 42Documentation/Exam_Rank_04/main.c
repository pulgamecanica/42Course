#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#define END		0
#define PIPE		1
#define SEMICOLON	2

#ifdef CHECK
#define	malloc(x) (void *)0
#define calloc(x1, x2) (void *)0
#endif

typedef struct	s_list
{
	struct s_list	*next;
	struct s_list	*prev;
	char		**args;
	int			pipe[2];
	int			type;
	int			redirect;
}	t_list;

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

int	arrlen(char **list)
{
	int	i;

	i = 0;
	if (!list)
		return (0);
	while (list[i])
		i++;
	return (i);
}

void	free_arr(char **list)
{
	int	i;

	i = 0;
	if (!list)
		return ;
	while (list[i])
		free(list[i++]);
	free(list);
}

void	free_list(t_list *list)
{
	t_list	*tmp;
	t_list	*tmp_free;

	if (!list)
		return ;
	tmp = list;
	while (tmp)
	{
		tmp_free = tmp;
		tmp = tmp->next;
		free_arr(tmp_free->args);
		free(tmp_free);
	}
}

char	*ft_strdup(char *str)
{
	char	*res;	
	int	i;

	i = 0;
	res = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!res)
		return (NULL);
	res[ft_strlen(str)] = 0;
	while (i < ft_strlen(str))
	{
		res[i] = str[i];
		i++;
	}
	return (res);
}

void	add_arg(char ***args, char *arg)
{
	char	**tmp;
	int	i;

	i = 0;
	if (!arg)
		return ;
	tmp = (char **)malloc(sizeof(char *) * (arrlen(*args) + 2));
	if (!tmp)
		return ;
	tmp[arrlen(*args) + 1] = 0;
	while (i < arrlen(*args))
	{
		tmp[i] = ft_strdup((*args)[i]);
		i++;
	}
	tmp[i] = ft_strdup(arg);
	free_arr(*args);
	*args = tmp;
}

t_list	*init_list(t_list *prev)
{
	t_list	*list;

	list = (t_list *)malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	list->type = END;
	list->prev = prev;
	list->next = NULL;
	list->args = NULL;
	list->redirect = 0;
	return (list);
}

t_list	*generate_list(int ac, char **av)
{
	t_list  *list;
	t_list	*tmp;
	int	i;

	list = init_list(NULL);
	tmp = list;
	i = 1;
	while (i < ac && list)
	{
		if (strcmp(av[i], "|") == 0)
		{
			tmp->type = PIPE;
			tmp->next = init_list(tmp);
			tmp = tmp->next;
			tmp->redirect = 1;
			i++;
			continue ;
		}
		else if (strcmp(av[i], ";") == 0)
		{
			tmp->type = SEMICOLON;
			tmp->next = init_list(tmp);
			tmp = tmp->next;
			i++;
			continue ;
		}
		add_arg(&(tmp->args), av[i]);
		i++;
	}
	return (list);
}

void	exec_cmd(t_list *cmd, char **env)
{
	int	pid;
	int	pipe_open;

	pipe_open = 0;
	if (cmd->type == PIPE || cmd->redirect)
	{
		pipe_open = 1;
		if (pipe(cmd->pipe))
			return ;
	}
	pid = fork();
	if (pid < 0)
		return ;
	if (pid == 0)
	{
		if (cmd->type == PIPE)
			if (dup2(cmd->pipe[1], STDOUT_FILENO) < 0)
				return ;
		if (cmd->redirect)
			if (dup2(cmd->prev->pipe[0], STDIN_FILENO) < 0)
				return ;
		execve(cmd->args[0], cmd->args, env);
		write(2, "error: cannot execute ", ft_strlen("error: cannot execute "));
		write(2, cmd->args[0], ft_strlen(cmd->args[0]));
		write(2, "\n", 1);
	}
	else
	{
		wait(NULL);
		if (pipe_open)
		{
			close(cmd->pipe[1]);
			if (!cmd->next || cmd->type == SEMICOLON)
				close(cmd->pipe[0]);
		}
		if (cmd->redirect)
			close(cmd->prev->pipe[0]);
	}
}

void	exec_cmds(t_list *list, char **env)
{
	t_list *tmp;

	tmp = list;
	while (tmp)
	{
		if (!tmp->args)
		{
			tmp = tmp->next;
			continue ;
		}
		if (strcmp("cd", tmp->args[0]) == 0)
		{
			if (arrlen(tmp->args) < 2)
				printf("error: cd: bad arguments\n");
			else if (chdir(tmp->args[1]))
				printf("error: cd: cannot change directory to %s\n", tmp->args[1]);
		}
		else
			exec_cmd(tmp, env);
		tmp = tmp->next;
	}
	(void)env;
}

int main(int ac, char **av, char **env)
{
	t_list	*list;
	int	i;

	if (ac < 2)
		return (0);
	list = generate_list(ac, av);
	if (list)
		exec_cmds(list, env);
	free_list(list);
	return (0);
}
