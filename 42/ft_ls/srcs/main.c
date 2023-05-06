#include "ft_ls.h"

void	print_list(void * ptr) {
	char * str;

	str = (char *)ptr;
	ft_printf("%s%s%s\n", YELLOW, str, ENDC);
}


int	init_program(t_list ** list, t_conf * conf, int ac, char *av[]) {
	int i;

	i = 0;
	while (++i < ac) {
		if (av[i][0] == '-' && !conf->delimit) {
			printf("\tAdding flag:\033[0;34m %s \033[0m\n", av[i]);
			if (!add_flags(conf, av[i]))
				return (EXIT_FAILURE);
		} else {
			printf("\tAdding path:\033[0;32m %s \033[0m\n", av[i]);
			ft_lstadd_back(list, ft_lstnew(ft_strdup(av[i])));
		}
	}
	if (!*list)
		ft_lstadd_back(list, ft_lstnew(ft_strdup(".")));
	return (EXIT_SUCCESS);
}

int	main(int ac, char *av[]) {
	int exit_status;
	t_conf *	conf;
	t_list ** param_files;

	conf = init_conf();
	if (!conf)
		return (EXIT_FAILURE);
	param_files = (t_list **)ft_calloc(sizeof(t_list *), 1);
	if (!param_files) {
		free(conf);
		return (EXIT_FAILURE);
	}
	exit_status = init_program(param_files, conf, ac, av);
	if (!exit_status) {
		if (DEBUG)
			print_conf(conf);
		ft_lstiter(*param_files, print_list);
	}
	ft_lstclear(param_files, free);
	free(conf);
	free(param_files);
	return (EXIT_SUCCESS);
}