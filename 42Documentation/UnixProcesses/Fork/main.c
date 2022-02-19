// gcc main.c -I ../../Libft_Ultimate/includes/ -L ../../Libft_Ultimate/ -lft && ./a.out 
#include <unistd.h>
#include <sys/wait.h>
#include "ft_printf.h"

void	multiple_processes(int	pid)
{
	int	id1;
	int	id2;
	int	status1;
	int	status2;

	if (pid == 0)
		return ;
	id1 = fork(); // Main process creates a child process
	id2 = fork(); // Main procces and child of main proces create a child process

	if (id1 == 0)
	{
		if (id2 == 0)
			ft_printf("Proinf from the grandchild process this is the deepest process (%d)\n", getpid());
		else
			ft_printf("Printf from first child (%d)\n", getpid());
	}
	else
	{
		if (id2 == 0)
			ft_printf("Printf from the second child process (%d)\n", getpid());
		else
			ft_printf("Printf from the main process (%d)\n", getpid());
	}

	status2 = WIFEXITED(id2);
	if (id1 != 0)
	{
		status1 = WIFEXITED(id1);
		wait(&status1);
		if (id2 != 0)
			wait(&status2);
	}
	if (id2  != 0)
		wait(&status2);
}

void	execute_child_first(int	id, int num)
{
	int	status;

	status = WIFEXITED(id);
	if (id != 0)
		wait(&status);
	while (num--)
		ft_printf("%d ", num + 1);
	write(1, "\n", 1);
}

int	main(void)
{
	int	id;
	int	num;

	ft_printf("This Project will prioritize the child proccess always and print the sequence of the child first.\n");
	id = 0;
	id = fork();
	if (id == 0)
	{
		num = 10;
		ft_printf("Child calling printf with an id <%d>\n", id);
	}
	else
	{
		num = 5;
		ft_printf("Main proccess calling printf with fork procces id: <%d>\n", id);
	}
	execute_child_first(id, num); // This function will first WAIT for the child proccess to finish, then it will continue!
	multiple_processes(id);
	return (0);
}
