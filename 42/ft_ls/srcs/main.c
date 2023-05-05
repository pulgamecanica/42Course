#include "ft_ls.h"

void	print_list(void * ptr) {
	char * str;

	str = (char *)ptr;
	ft_printf("%s%s%s\n", YELLOW, str, ENDC);
}



int	main(int ac, char *av[]) {
	int exit_status;
	int i;
	t_list * param_files;
	t_conf	conf;

	exit_status = init_conf(&conf);
	if (exit_status != EXIT_SUCCESS)
		return (exit_status);

	i = 0;
	while (++i < ac) {
		if (av[i][0] == '-' && !conf.delimit) {
			printf("\tAdding flag:\033[0;34m %s \033[0m\n", av[i]);
			if (!add_flags(&conf, av[i]))
				return (EXIT_FAILURE);
		} else {
			printf("\tAdding path:\033[0;32m %s \033[0m\n", av[i]);
			ft_lstadd_back(&param_files, ft_lstnew(av[i]));
		}
	}
	if (DEBUG)
		print_conf(&conf);

	ft_lstiter(param_files, print_list);
	return (exit_status);
}