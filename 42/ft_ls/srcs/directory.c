#include <sys/types.h>
#include <dirent.h>
#include "file.h"
#include "conf.h"
#include "libft.h"
#include "ft_ls.h"
#include "ft_printf.h"

void	recursive_directory_listing(void * ptr1, void * ptr2) {
	t_file * f;
	t_conf * c;

	f = (t_file *)ptr1;
	c = (t_conf *)ptr2;
	if (!f || !c)
		return ;
	if (f->fit == DIRECTORY && ft_strcmp(f->name, "..") != 0 && ft_strcmp(f->name, ".") != 0)
		list_directory(f, c);
}

void	list_directory(void * ptr1, void * ptr2) {
	t_list ** files;
	t_file * f;
	t_file * tmp;
	t_conf * c;
	DIR	* dir;
	char * full_path;
	struct dirent * ent;

	f = (t_file *)ptr1;
	c = (t_conf *)ptr2;
	if (!f || !c)
		return ;
	if (!(files = (t_list **)ft_calloc(sizeof(t_file *), 1)))
		return ;
	// Read directory
	// Save entries in another list
	full_path = ft_strjoin(ft_strdup(f->path ? f->path : ""), f->name);
	dir = opendir(full_path);
	while ((ent = readdir(dir))) {
		if (*(ent->d_name) == '.') {
				if (!c->no_ignore && !c->almost_no_ignore)
					continue ;
				else if (c->almost_no_ignore && (*(ent->d_name + 1) == 0 || *(ent->d_name + 1) == '.'))
					continue ;
		}
		tmp = new_file(ent->d_name, full_path);
		if (tmp) {
			ft_lstadd_back(files, ft_lstnew(tmp));
		}
	}
	free(full_path);
	closedir(dir);
	// Sort entries
	ft_lstsort(files, c->sorting_f);
	// Print entries
	if (!c->initial_separation)
		c->initial_separation = true;
	else
		write(1, "\n", 1);
	if (c->print_dir) {
		ft_putstr_fd(f->path, 1);
		ft_putstr_fd(f->name, 1);
		ft_putstr_fd(":\n", 1);
	}


	if (c->size || c->format == long_format) {
		int total;
		total = 0;
		ft_lstiter_param(*files, setup_total, &total);
	 	ft_printf("total %d\n", total);
	}
	
	// Setup the padding for the file listing
	// Reset padding first
	ft_bzero(&c->padding, sizeof(t_padding));
	ft_lstiter_param(*files, setup_padding, c);

	ft_lstiter_param(*files, c->format_f, c);
	
	if (c->recursive)
		ft_lstiter_param(*files, recursive_directory_listing, c);
	
	ft_lstclear(files, free_file);
	free(files);
}