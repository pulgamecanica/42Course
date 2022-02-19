#include <errno.h>
#include <sys/wait.h>
#include "ft_printf.h"

int	main(int ac, char **av)
{
	int	fds[2];
	int	id;
	int	status;
	int	array[] = {10, 5, 6, 1, 1, 4, 7, 4, 5, 9};
	int	len = 10;
	int	start;
	int	rc;
	int	buff;
	int	pstat;

	status = pipe(fds);
	if (status == -1)
	{
		perror("Pipe Error: ");
		return (0);
	}
	id = fork();
	if (id == -1)
	{
		perror("Fork Error: ");
		return (0);
	}
	else if (id == 0)
	{
		close(fds[0]); // close the "reading" end of the pipe
		start = len / 2;
	}
	else
	{
		close(fds[1]); // close the "write" end of the pipe
		start = 0;
		len = len / 2;
		pstat = WIFEXITED(id);
		wait(&pstat);
	}
	while (start < len)
		status += array[start++];
	ft_printf("My job here is done! The sum is: %d \n", status);
	if (id == 0)
	{
		write(fds[1], &status, sizeof(int));
                close(fds[1]);
	}
	else
	{
		rc = read(fds[0], &buff, sizeof(int));
		if (rc == -1)
		{
			perror("Reading fd Error: ");
			return (0);
		}
		status += buff;
		ft_printf("The sum overall is: %d\n", status);
		close (fds[0]);
	}
	return (0);
}
