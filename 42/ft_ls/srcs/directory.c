#include "file.h"
#include "conf.h"
#include "libft.h"
#include "ft_ls.h"
#include "ft_printf.h"
#include <stdint.h>
#include <dirent.h>

static void	recursive_directory_listing(void * ptr1, void * ptr2) {
	t_file * f;
	t_conf * c;

	f = (t_file *)ptr1;
	c = (t_conf *)ptr2;
	if (!f || !c)
		return ;
	if (f->fit == DIRECTORY && ft_strcmp(f->name, "..") != 0 && ft_strcmp(f->name, ".") != 0)
		list_directory(f, c);
}

static t_list ** directory_entries(t_conf * conf, t_file * file) {
	t_list ** entries;
	char * full_path;
	t_file * tmp;
	struct dirent	*	ent;
	DIR	*	dir;

	if (!(entries = (t_list **)ft_calloc(sizeof(t_file *), 1)))
		return (NULL);
	full_path = ft_strjoin(ft_strdup(file->path ? file->path : ""), file->name);
	dir = opendir(full_path);
	if (!dir) {
		ft_printf("ls: cannot open directory '%s': Permission denied\n", full_path);
		free(full_path);
		return (NULL);
	}
	while ((ent = readdir(dir))) {
		if (*(ent->d_name) == '.') {
				if (!conf->no_ignore && !conf->almost_no_ignore)
					continue ;
				else if (conf->almost_no_ignore && (*(ent->d_name + 1) == 0 || *(ent->d_name + 1) == '.'))
					continue ;
		}
		tmp = new_file(ent->d_name, full_path);
		if (tmp)
			ft_lstadd_back(entries, ft_lstnew(tmp));
	}
	free(full_path);
	closedir(dir);
	ft_lstsort(entries, conf->sorting_f);
	return (entries);
}

/**
 * Pre Listing, separation and direcoty path
 * Initial separation is only needed when other entries were listed before
 * This will prevent an initial separation when nothing else was printed before the listing
 * Print the path followed by ':' when there is more than one parameter, or it is recursive
 * Print total, when it's long format, or when the block size is visible
 **/
static void print_pre_listing(t_conf * c, t_file * f, t_list ** files) {
	uintmax_t total;

	if (!c->initial_separation)
		c->initial_separation = true;
	else
		write(1, "\n", 1);
	if (c->print_dir) {
		ft_putstr_fd(f->path, 1);
		ft_putstr_fd(f->name, 1);
		ft_putstr_fd(":\n", 1);
	}
	if (c->block_size || c->format == long_format) {
		total = 0;
		ft_lstiter_param(*files, setup_total, &total);
	 	ft_printf("total %d\n", ft_ceil(total / 2.0));
	}
}

/**
 * This function is called on ft_lstiter_param 
 * list_directory should only receive DIRECTORIES
 * as file entries, any other option is undefined.
 **/
void	list_directory(void * ptr1, void * ptr2) {
	t_list ** files;
	t_file * f;
	t_conf * c;

	f = (t_file *)ptr1;
	c = (t_conf *)ptr2;
	if (!f || !c)
		return ;
	/* Read directory & Save entries in another list */
	if (!(files = directory_entries(c, f)))
		return ;
	// Print separation, directory path and total size when needed
	print_pre_listing(c, f, files);
	// Setup Scontext if needed
	if (c->scontext || c->format == long_format)
		ft_lstiter(*files, setup_scontext);
	// Setup the padding for the file listing, reset padding first
	ft_bzero(&c->padding, sizeof(t_padding));
	ft_lstiter_param(*files, setup_padding, c);
	// Print the entries
	ft_lstiter_param(*files, c->format_f, c);
	// When recursive, all the directory entries should be listed
	if (c->recursive)
		ft_lstiter_param(*files, recursive_directory_listing, c);
	ft_lstclear(files, free_file);
	free(files);
}
