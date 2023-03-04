#include "ft_ls.h"

int cmp_not_exist_first_directory_last(void * ptr1, void * ptr2) {
	t_file * f1, * f2;

	f1 = (t_file *)ptr1;
	f2 = (t_file *)ptr2;
	if (!f1 || !f2)
		ft_exit(1, "Bad Error invalid (void *) cast\n", 0);
	
	if (f1->f_errors == NotFounded) {
		if (f2->f_errors == NotFounded)
			return -1;
		return 1;
	}
	if (((f1->f_stat.st_mode & S_IFMT) != S_IFDIR && (f2->f_stat.st_mode & S_IFMT) != S_IFDIR)
		|| ((f1->f_stat.st_mode & S_IFMT) == S_IFDIR && (f1->f_stat.st_mode & S_IFMT) == S_IFDIR))
		return (cmp_ascii_order(ptr1, ptr1));
	else if ((f1->f_stat.st_mode & S_IFMT) == S_IFDIR)
		return -1;
	else
		return 1;
	return 0;
}

int cmp_ascii_order(void * ptr1, void * ptr2) {
	t_file * f1, * f2;

	f1 = (t_file *)ptr1;
	f2 = (t_file *)ptr2;
	if (!f1 || !f2)
		ft_exit(1, "Bad Error invalid (void *) cast\n", 0);
	//if (DEBUG)
	//	ft_printf("Compare %s vs %s [%d]\n", f1->f_name, f2->f_name, ft_strcmp_insensitive(f1->f_name, f2->f_name));
	return ft_strcmp_insensitive(f1->f_name, f2->f_name);
}