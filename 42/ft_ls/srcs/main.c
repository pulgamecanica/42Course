#include "ft_ls.h"

void	print_list(void * ptr) {
	t_file * file;

	file = (t_file *)ptr;
	ft_printf("\t%s[%c] %s%s%s\n", YELLOW, file->fileType, file->path, file->name, ENDC);
}

int	init_program(t_list ** list, t_conf * conf, int ac, char *av[]) {
	int i;

	i = 0;
	while (++i < ac) {
		if (av[i][0] == '-' && !conf->delimit) {
			if (DEBUG)
				printf("\tAdding flag:\033[0;34m %s \033[0m\n", av[i]);
			if (!add_flags(conf, av[i]))
				return (EXIT_FAILURE);
		} else {
			if (DEBUG)
				printf("\tAdding path:\033[0;32m %s \033[0m\n", av[i]);
			ft_lstadd_back(list, ft_lstnew(setup_file(av[i], "")));
		}
	}
	if (!ft_lstsize(*list))
		ft_lstadd_back(list, ft_lstnew(setup_file(".", "")));
	conf->print_dir = ft_lstsize(*list) > 1 || conf->recursive; 
	return (EXIT_SUCCESS);
}

int	ft_exit(t_conf * conf, t_list ** files1, t_list ** files2) {
	if (conf) {
		free(conf);
	}
	if (files1) {
		ft_lstclear(files1, free_file);
		free(files1);
	}
	if (files2) {
		ft_lstclear(files2, free_file);
		free(files2);
	}
	return (EXIT_FAILURE);
}

int	main(int ac, char *av[]) {
	t_conf *	conf;
	t_list ** param_files;
	t_list ** pending_directories;

	pending_directories = NULL;
	param_files = (t_list **)ft_calloc(sizeof(t_list *), 1); // ALLOCATE PARAMS LIST
	if (!param_files)
		return (EXIT_FAILURE);

	conf = init_conf(); // INITIALIZE CONFIGURATION WITH DEFAULT VALUES
	if (!conf)
		return (ft_exit(conf, param_files, pending_directories));
	if (init_program(param_files, conf, ac, av) != EXIT_SUCCESS) // ADDING PARAMETER FILES AND FLAGS
		return (ft_exit(conf, param_files, pending_directories));

	conf->format = LongFormat; //TESTING
	
	if (!conf->no_explore) { // If Falg -d is not active, add all directories to the pending list of directories
		pending_directories = extract_directories(*param_files);
		if (!pending_directories)
			return (ft_exit(conf, param_files, pending_directories));
		if (ft_lstsize(*pending_directories)) {
			ft_lstsort(pending_directories, cmp_ascii_order);
		}
	}

	if (DEBUG) { // HANDLE & PRINT DEBUG SUFF
		print_conf(conf);
		ft_printf("Parameters\n");
		ft_lstiter(*param_files, print_list);
		if (pending_directories && ft_lstsize(*pending_directories)) {
			ft_printf("Pending Directories\n");
			ft_lstiter(*pending_directories, print_list);
		}
	}

	set_padding(*param_files, conf);
	ft_lstiter_param(*param_files, print_files, conf);
	conf->params_on = false;
	if (pending_directories && ft_lstsize(*pending_directories)) {
		if (ft_lstsize(*param_files) != ft_lstsize(*pending_directories))
				ft_putchar_fd('\n', 1);
		t_list * tmp = *pending_directories;
		while (tmp) {
			if (conf->print_dir)
				ft_printf("%s:\n", ((t_file *)(tmp->content))->name);
			read_directories(tmp->content, conf);
			if (tmp->next)
				ft_putchar_fd('\n', 1);
			//ft_lstiter_param(*pending_directories, read_directories, conf);
			tmp = tmp->next;
		}
	}

	// FREE STUFF
	ft_lstclear(param_files, free_file);
	free(conf);
	free(param_files);
	if (pending_directories) {
		ft_lstclear(pending_directories, free_file);
		free(pending_directories);
	}
	return (EXIT_SUCCESS);
}