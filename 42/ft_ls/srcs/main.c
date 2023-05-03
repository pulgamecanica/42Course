#include "ft_ls.h"

int	main(int ac, char *av[]) {
	int exit_status;
	int _t;
	t_conf	conf;

	exit_status = init_conf(&conf);
	if (exit_status != EXIT_SUCCESS)
		return (exit_status);

	_t = 0;
	while (++_t < ac) {
		if (av[_t][0] == '-' && !conf.delimit) {
			printf("\tAdding flag:\033[0;34m %s \033[0m\n", av[_t]);
			if (!add_flags(&conf, av[_t]))
				return (EXIT_FAILURE);
		} else {
			printf("\tAdding path:\033[0;32m %s \033[0m\n", av[_t]);
		}
	}
	if (DEBUG)
		print_conf(&conf);
	return (exit_status);
}