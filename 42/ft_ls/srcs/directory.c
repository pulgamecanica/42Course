#include "libft.h"
#include "ft_ls.h"

static void	prot_exclude_dirs(void * ptr1, void * ptr2) {
	t_file * file;

	file = (t_file *)ptr1;
	if (file->fileType == Directory)
		ft_lstadd_back(ptr2, ft_lstnew(setup_file(file->name, file->path)));
}

t_list **	extract_directories(t_list * list) {
	t_list ** pending_directories;

	if (!list)
		return (NULL);
	pending_directories = (t_list **)ft_calloc(sizeof(t_list *), 1);
	if (!pending_directories)
		return (NULL);
	ft_lstiter_param(list, prot_exclude_dirs, pending_directories);
	return (pending_directories);
}

void	read_directories(t_file * file, t_conf * conf) {
	t_list ** files;
	t_list ** pending_directories;
	DIR * dir;
	char * full_path;
	struct dirent * ent;

	files = (t_list **)ft_calloc(sizeof(t_list *), 1);
	if (!files)
		return ;
	full_path = ft_strjoin(ft_strdup((file->path != NULL) ? file->path : ""), file->name);
	dir = opendir(full_path);
	while ((ent = readdir(dir))) {
		if (*ent->d_name == '.' && !conf->no_ignore)
			continue ;
		if (DEBUG)
			printf("[%s] %s\n\td_ino [%lu]\n\td_off [%lu]\n\td_reclen [%u]\n\td_type [%d]\n", full_path, ent->d_name, ent->d_ino, ent->d_off, ent->d_reclen, ent->d_type);
		ft_lstadd_back(files, ft_lstnew(setup_file(ent->d_name, full_path)));
	}
	closedir(dir);
	free(full_path);
	ft_lstsort(files, cmp_ascii_order);
	if (files) {
		set_padding(*files, conf);
		printf("total %d\n", get_total_block_size(*files));
		ft_lstiter_param(*files, print_files, conf);
	}

	if (conf->recursive) {
		pending_directories = extract_directories(*files);
		if (pending_directories && ft_lstsize(*pending_directories)) {
				t_list * tmp = *pending_directories;
				while (tmp) {
					if (ft_strcmp(((t_file *)(tmp->content))->name, ".") == 0 || ft_strcmp(((t_file *)(tmp->content))->name, "..") == 0) {
						tmp = tmp->next;
						continue ;
					}
					ft_putchar_fd('\n', 1);
					ft_printf("%s%s:\n", ((t_file *)(tmp->content))->path, ((t_file *)(tmp->content))->name);
					read_directories(tmp->content, conf);
					tmp = tmp->next;
				}
			}
		if (pending_directories && ft_lstsize(*pending_directories))
			ft_lstclear(pending_directories, free_file);
		free(pending_directories);
	}
	ft_lstclear(files, free_file);
	free(files);
}