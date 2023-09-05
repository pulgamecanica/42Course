#include "conf.h"
#include "file.h"
#include "libft.h"
#include "ft_ls.h"

/* Bait function to pass to the delete function, this way we can move elements */
static void	bait(void * ptr) {
	(void)ptr;
}

/**
 * This function will move the files which should not be listed to a new list.
 * The new list is returned and the files moved are removed from the list passed.
 **/
static t_list ** extract_param_files_to_print(t_list **head, t_conf * conf) {
	t_list ** list;
	t_list * tmp;
	t_list * tmp2;
	t_file * tmp_file;

	if (!head || !conf)
		return (NULL);
	if (!(list = (t_list **)ft_calloc(sizeof(t_list *), 1)))
		return (NULL);
	tmp = *head;
	while (tmp) {
		tmp2 = tmp;
		tmp = tmp->next;
		tmp_file = (t_file *)tmp2->content;
		if (!tmp_file) // If any file was unsuccesfull the error will be displayed and a separation will be needed
			conf->initial_separation = true;
		if (conf->no_explore || (tmp_file && tmp_file->fit != DIRECTORY)) {
			conf->initial_separation = true;
			ft_lstadd_back(list, ft_lstnew(tmp2->content));
			ft_lstdelelem(head, tmp2, bait);
		}
	}
	return (list);
}

/**
 * Exit and free all allocated memory 
 **/
static void	ft_exit(int status, t_conf * conf, t_list ** list) {
 if (conf)
 	free(conf);
 if (list) {
 	ft_lstclear(list, free_file);
 	free(list);
 }
 exit(status);
}

/**
 * ls will always list first the argument files 
 * Then it will list each argument directory
 * 
 * If any files are listed, then the directory name should
 * be printed for any future directory listings.
 **/
static void list_argument_files(t_conf * conf, t_list ** param_directories){
		t_list ** param_files;

		if (!(param_files = extract_param_files_to_print(param_directories, conf)))
			ft_exit(2, conf, param_directories);
		ft_lstiter_param(*param_files, setup_padding, conf);
		ft_lstiter_param(*param_files, conf->format_f, conf);
		conf->print_dir = conf->recursive || ft_lstsize(*param_files) > 0 || ft_lstsize(*param_directories) > 1;
		ft_lstclear(param_files, free_file);
		free(param_files);
}

/**
 * Parsing parameters
 * Read all the options and directories 
 * Return error and exit with all the proper frees
 **/
static void	parse_parameters(int ac, char * av[], t_conf * conf, t_list ** list) {
	for (int i = 1; i < ac; i++) {
		if (is_flag(av[i]) && !conf->delimit) {
			if (!add_flag(av[i], conf)) {
				ft_exit(2, conf, list);
			}
		} else {
			ft_lstadd_back(list, ft_lstnew(new_file(av[i], NULL)));
		}
	}
}

/**
 * List  information  about  the  FILEs  (the current directory by default).
 * Sort entries  alphabetically  if  none  of  -cftuvSUX  nor --sort is specified
 * 
 * Return 0 on success
 * Return 1 on minor issues (file permissions)
 * Return 2 on serious trouble, unexpected behaviours, returns
**/
int main(int ac, char *av[])
{
	t_conf	* conf;
	t_list	** param_directories;

	if (!(param_directories = (t_list **)ft_calloc(sizeof(t_list *), 1)))
		ft_exit(2, NULL, NULL);
	if (!(conf = new_default_conf_ls()))
		ft_exit(2, NULL, param_directories);
	parse_parameters(ac, av, conf, param_directories);
	// Evaluate outcome, asign the sorting function, assign the format function
	conf->sorting_f = sorting_func(conf->sorting, conf->sort_rev);
	conf->format_f = format_func(conf->format); 
	if (ft_lstsize(*param_directories) == 0)
		ft_lstadd_back(param_directories, ft_lstnew(new_file(".", NULL)));
	// Sort and print file entries that are not going to be listed, list all directorie
	ft_lstsort(param_directories, conf->sorting_f);
	list_argument_files(conf, param_directories);
	ft_lstiter_param(*param_directories, list_directory, conf);
	ft_exit(0, conf, param_directories);
}
