#include "ft_ls.h"

void free_file(void * ptr) {
  t_file * file;

  file = (t_file *)ptr;
  if (file->children)
    ft_lstclear(&file->children, free_file);
	free(file->f_name);
	free(file->f_path);
	free(ptr);
}

void	ft_print_files(t_list * head, int depth, bool various) {
	t_file	* file;

	if (head) {
		file = (t_file *)head->content;
		if (!file)
			ft_exit(1, "Bad Error invalid (void *) cast\n", 0);
		// Probably good to check flagD here...
		if (file->f_errors == PermissionDenied) {
			ft_printf("ls: cannot open directory '%s%s': Permission denied\n", (file->f_path != NULL) ? file->f_path : "", file->f_name);
		} else if (file->f_errors == NotFounded) {
			ft_printf("ls: cannot access '%s%s': No such file or directory\n", (file->f_path == NULL) ? "" : file->f_path, file->f_name);
		}
		//ft_print_file(head->content);
		if (file->f_errors == NoError) {
			if ((!depth && (file->f_stat.st_mode & S_IFMT) != S_IFDIR) || depth) {
				if (head->next) {
					ft_printf("%s  ", file->f_name);
				} else {
					ft_printf("%s\n", file->f_name);
				}
			}
		}
		ft_print_files(head->next, depth, various);
		if (file->children && (file->f_stat.st_mode & S_IFMT) == S_IFDIR) {
			if (head->next) {
				ft_printf("\n");
			}
			if ((various || depth) && (!various || !depth))
				ft_printf("%s%s:\n", (file->f_path == NULL) ? "" : file->f_path, file->f_name);
			ft_print_files(file->children, ++depth, various);
		}
	}
}

t_file * init_file(char * str, char * path) {
  t_file *  file;

  file = (t_file *)ft_calloc(sizeof(t_file), 1);
  if (!file)
    return NULL;
	file->f_errors = 0;
  file->f_errors = NoError;
  file->f_name = ft_strdup(str);
  if (!file->f_name) {
    free(file);
    return NULL;
  }
	if (!path)
		file->f_path = NULL;
	else {
		if (path[ft_strlen(path) - 1] == '/')
			file->f_path = ft_strdup(path);
		else
			file->f_path = ft_strjoin(ft_strdup(path), "/");
	}
  file->total = 0;
  file->children = NULL;
  return (file);
}

