#include "libft.h"
#include "ft_ls.h"

static void	prot_exclude_dirs(void * ptr1, void * ptr2) {
	t_file * file;

	file = (t_file *)ptr1;
	if (file->fileType == Directory)
		ft_lstadd_back(ptr2, ft_lstnew(setup_file(file->name, "")));
}

t_list **	extract_directories(t_list * list) {
	t_list ** pending_directories;

	pending_directories = (t_list **)ft_calloc(sizeof(t_list *), 1);
	if (!pending_directories)
		return (NULL);


	ft_lstiter_param(list, prot_exclude_dirs, pending_directories);
	return (pending_directories);
}