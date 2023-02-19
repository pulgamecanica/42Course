#include "ft_ls.h"

/*
int     ft_lstsize(t_list *lst);
void    ft_lstadd_front(t_list **lst, t_list *new);
void    ft_lstadd_back(t_list **lst, t_list *new);
void    ft_lstdelone(t_list *lst, void (*del)(void *));
void    ft_lstdelelem(t_list **lst, t_list *elem, void (*del)(void *));
void    ft_lstclear(t_list **lst, void (*del)(void *));
void    ft_lstiter(t_list *lst, void (*f)(void *));
t_list    *ft_lstnew(void *content);
t_list    *ft_lstlast(t_list *lst);
t_list    *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

typedef struct s_file {
  enum filetype f_type;
  size_t    f_size;
  size_t    f_create_date;
  size_t    f_last_modify;
  char *    f_name;
  short int f_mode;
  int       total;
  t_list *  children;
} t_file;


typedef struct ls_config {
	ls_flags	flags;
	t_list *	files;
}	ls_file;
*/

void free_file(void * ptr) {
  t_file * file;

  file = (t_file *)ptr;
  if (file->children)
    ft_lstclear(&file->children, free_file);
	free(file->f_name);
	free(file->f_path);
	free(ptr);
}

static void ft_print_file(t_file * file) {
	ft_printf(""
	"type: %d, "
	"path: %-12.12s, "
	"name: %-12.12s, "
	"mode: %0.3d, "
	"size: %d, "
	"total: %d, "
	"created_at: %d, "
	"last_modify: %d, "
	"# children: %d",
	file->f_type,
	file->f_path,
	file->f_name,
	file->f_mode,
	file->f_size,
	file->total,
	file->f_create_date,
  file->f_last_modify,
	ft_lstsize(file->children));
}

void	setup_file(void * ptr, void * ptr2) {
	t_file * file, *	tmp;
	ls_flags * flags;
	DIR * dir;
	struct dirent * ent;
	char * full_path;

	flags = (ls_flags *)ptr2;
	file = (t_file *)ptr;
	full_path = ft_strjoin(ft_strdup((file->f_path != NULL) ? file->f_path : ""), file->f_name);
	dir = opendir(full_path);
//	if (file->path)
//		dir	= opendir(file->f_name);
	if (!dir) {
		file->f_type = NotFounded;
		if (DEBUG)
			ft_printf("File %s%s not founded :(\n", (file->f_path == NULL) ? "" : file->f_path, file->f_name);
		free(full_path);
		return ;
	}
	if (DEBUG)
		ft_printf("Opened file %s%s, setting up children :)\n", (file->f_path == NULL) ? "" : file->f_path, file->f_name);
	while ((ent = readdir(dir))
		&& ft_strncmp(file->f_name, "..", ft_max(ft_strlen(file->f_name), ft_strlen("..")))
		&& ft_strncmp(file->f_name, ".", ft_max(ft_strlen(file->f_name), ft_strlen(".")))
	) {
		if (ent->d_name[0] == '.' && !flags->flaga)
			continue ;
		if (DEBUG)
			printf("children %s\n\td_ino [%lu]\n\td_off [%lu]\n\td_reclen [%u]\n\td_type [%d]\n", ent->d_name, ent->d_ino, ent->d_off, ent->d_reclen, ent->d_type);
		tmp = init_file(ent->d_name, full_path);
		if (!tmp) {
			free(full_path);
			return ;
		}
		tmp->f_size = ent->d_reclen;
		tmp->d_ino = ent->d_ino;
		ft_lstadd_front(&file->children, ft_lstnew(tmp));
	}
	free(full_path);
	closedir(dir);
	if (flags->flagR && file->children)
		ft_lstiter_param(file->children, setup_file, ptr2);
}

//void	setup_files(ls_config * config) {
//	if (!config)
//		return;
//	ft_lstiter(config->files, setup_file);
	// if flag R and childre
	// ft_lstiter(congig., void (*f)(void *));
//}

void	ft_print_files(t_list * head) {
	if (!head)
		return ;
	ft_print_file(head->content);
	ft_printf("\n");
	ft_print_files(head->next);
}

t_file * init_file(char * str, char * path) {
  t_file *  file;

  file = (t_file *)ft_calloc(sizeof(t_file), 1);
  if (!file)
    return NULL;
	file->f_type = 0;
  file->f_size = 0;
  file->f_create_date = 0;
  file->f_last_modify = 0;
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
  file->f_mode = 0;
  file->total = 0;
  file->children = NULL;
  return (file);
}

