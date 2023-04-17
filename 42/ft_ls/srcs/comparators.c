#include "ft_ls.h"

int cmp_not_exist_first(void * ptr1, void * ptr2) {
	t_file * f1, * f2;

	f1 = (t_file *)ptr1;
	f2 = (t_file *)ptr2;
	if (!f1 || !f2)
		ft_exit(1, "Bad Error invalid (void *) cast\n", 0);
	if (f1->f_errors == NotFounded || f2->f_errors == NotFounded) {
		if (f1->f_errors == NotFounded && f2->f_errors == NotFounded)
			return 0;
		if (f1->f_errors == NotFounded)
			return 1;
		return -1;
	}
	return (cmp_ascii_order(ptr2, ptr1));
}

int cmp_rev_not_exist_first(void * ptr1, void * ptr2) {
	t_file * f1, * f2;

	f1 = (t_file *)ptr1;
	f2 = (t_file *)ptr2;
	if (!f1 || !f2)
		ft_exit(1, "Bad Error invalid (void *) cast\n", 0);
	if (f1->f_errors == NotFounded || f2->f_errors == NotFounded) {
		if (f1->f_errors == NotFounded && f2->f_errors == NotFounded)
			return 0;
		if (f1->f_errors == NotFounded)
			return 1;
		return -1;
	}
	return (cmp_ascii_order(ptr1, ptr2));
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
