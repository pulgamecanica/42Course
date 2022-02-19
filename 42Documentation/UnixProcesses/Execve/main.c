#include "ft_printf.h"
#include <unistd.h>
#include <sys/wait.h>

int	main(int ac, char **av, char **envp)
{
	int	pid;
	int	execstat;
	int	waitstat;

	if (ac != 2)
	{
		ft_printf("Wrong number of arguments, should be one\n");
		return (0);
	}
	pid = fork();
	if (pid == -1)
		perror("Fork Error!: ");
	if (pid == 0)
	{
		execstat = execve("/usr/bin/ls", av, envp);
		if (execstat == -1)
			ft_printf("Error, didn't execute!\n");
	}
	else
	{
		waitstat = WIFEXITED(pid);
		wait(&waitstat);
		ft_printf("Finished Executing!\n");
	}
	return (0);
}
