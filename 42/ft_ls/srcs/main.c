#include "ft_ls.h"

int	main(int ac, char *av[]) {
	int exit_status;
	(void)ac;
	(void)av;
	t_conf	conf;

	exit_status = init_conf(&conf);
	return (exit_status);
}