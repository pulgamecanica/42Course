#include "ft_ls.h"

/*
int			ft_lstsize(t_list *lst);
void		ft_lstadd_front(t_list **lst, t_list *new);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstdelelem(t_list **lst, t_list *elem, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstnew(void *content);
t_list		*ft_lstlast(t_list *lst);
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

typedef struct s_file {
	enum filetype	f_type;
	size_t		f_size;
	size_t		f_create_date;
	size_t		f_last_modify;
	char *		f_name;
	short int	f_mode;
	int				total;
	t_list *	children;
}	t_file;
*/

int	main(int ac, char * av[]) {
	ls_config	config;

	config.flags = (ls_flags){false, false, false, false, false, false};
	config.files = NULL;
	for (int i = 1; i < ac; i++)
		if (!assign_flags(&config.flags, av[i]))
			ft_lstadd_front(&config.files, ft_lstnew(init_file(av[i], NULL)));
	if (!config.files || ft_lstsize(config.files) == 0)
		ft_lstadd_front(&config.files, ft_lstnew(init_file(".", NULL)));
	// ADD the Logic here
	setup_files(&config);
	// Print debug info here
	if (DEBUG) {
		ft_print_files(config.files);
		print_flags(&config.flags);
	}
	ft_lstclear(&config.files, free_file);
	return 1;
}

