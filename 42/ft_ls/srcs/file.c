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
*/


void free_file(void * ptr) {
  t_file * file;

  file = (t_file *)ptr;
  if (file->children)
    ft_lstclear(&file->children, free_file);
	free(file->f_name);
	free(ptr);
}

static void ft_print_file(t_file * file) {
	ft_printf(""
	"type: %d, "
	"name: %s, "
	"mode: %0.3d, "
	"size: %d, "
	"total: %d, "
	"created_at: %d, "
	"last_modify: %d, "
	"# children: %d",
	file->f_type,
	file->f_name,
	file->f_mode,
	file->f_size,
	file->total,
	file->f_create_date,
  file->f_last_modify,
	ft_lstsize(file->children));
}

void	ft_print_files(t_list * head) {
	if (!head)
		return ;
	ft_print_file(head->content);
	ft_printf("\n");
	ft_print_files(head->next);
}

t_file * init_file(char * str) {
  t_file *  file;

  file = (t_file *)ft_calloc(sizeof(t_file), 1);
  if (!file)
    return NULL;
  file->f_size = 0;
  file->f_create_date = 0;
  file->f_last_modify = 0;
  file->f_name = ft_strdup(str);
  if (!file->f_name) {
    free(file);
    return NULL;
  }
  file->f_mode = 0;
  file->total = 0;
  file->children = NULL;
  return (file);
}

