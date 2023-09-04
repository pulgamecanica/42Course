#include "conf.h"
#include "libft.h"
#include "file.h"
#include "ft_ls.h"

static void	bait(void * ptr) {
	(void)ptr;
}

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
		if (!tmp_file)
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
 * Return 0 on success
 * Return 1 on minor issues (file permissions)
 * Return 2 on serious trouble, unexpected behaviours, returns
**/
int main(int ac, char *av[])
{
	t_conf	* conf;
	t_list	** param_directories;
	// t_file	* tmp;

	if (!(param_directories = (t_list **)ft_calloc(sizeof(t_list *), 1)))
		return (2);
	if (!(conf = new_default_conf_ls()))
		return (2);
	/**
	 * Parsing parameters
	 *	Read all the options and directories 
	 * 	Return error and exit with all the proper frees
	 **/
	for (int i = 1; i < ac; i++)
	{
		if (is_flag(av[i]) && !conf->delimit) {
			if (!add_flag(av[i], conf)) {
				ft_lstclear(param_directories, free_file);
				free(param_directories);
				free(conf);
				return (2);
			}
		} else {
			// tmp = new_file(av[i], NULL);
			// if (tmp)
				ft_lstadd_back(param_directories, ft_lstnew(new_file(av[i], NULL)));
		}
	}

	// Evaluate outcome
	conf->sorting_f = sorting_func(conf->sorting, conf->sort_rev);
	conf->format_f = format_func(conf->format);
	// conf->sorting_f = sorting_func(conf->sorting, conf->sort_rev);

	// If no file is provided, use the current directory
	if (ft_lstsize(*param_directories) == 0) {
		ft_lstadd_back(param_directories, ft_lstnew(new_file(".", NULL)));
	}
	// Sort entries EASY
	ft_lstsort(param_directories, conf->sorting_f);
	/**
	 * ls will always list first the argument files 
	 * Then it will list each argument directory
	 **/
	{
		// Print the files given as arguments 
		t_list ** param_files_to_print;
		if (!(param_files_to_print = extract_param_files_to_print(param_directories, conf))) {
			ft_lstclear(param_directories, free_file);
			free(param_directories);
			free(conf);
			return (2);
		}
		
		// Setup the padding for the file listing
		ft_lstiter_param(*param_files_to_print, setup_padding, conf);
		// Print the files entries
		ft_lstiter_param(*param_files_to_print, conf->format_f, conf);
		/**
		 * Very important, when there is any other entry
		 * besides a directory, print the directory name
		 * before listing it's files
		 **/
		conf->print_dir = conf->recursive || ft_lstsize(*param_files_to_print) > 0 || ft_lstsize(*param_directories) > 1;
		ft_lstclear(param_files_to_print, free_file);
		free(param_files_to_print);
	}
	// List directories HARDEST
	{
		/** The directories to be listed are located at:  param_directories
		 * Iterate the list and for each directory, list it's files, then print them
		 * For Recursive, keep listing until an end is reached
		 * Never follow symbolic links, it may cause an infinite cycle 
		 * Do List symbolic links which are given as parameters
		 **/
		ft_lstiter_param(*param_directories, list_directory, conf);
	}
	
	ft_lstclear(param_directories, free_file);
	free(param_directories);
	free(conf);
	return 0;
}