#include "ft_ls.h"

int cmp_ascii_order(void * ptr1, void * ptr2) {
	t_file * f1, * f2;

	f1 = (t_file *)ptr1;
	f2 = (t_file *)ptr2;
	if (!f1 || !f2)
		return 0;
	//if (DEBUG)
	//	ft_printf("Compare %s vs %s [%d]\n", f1->f_name, f2->f_name, ft_strcmp_insensitive(f1->f_name, f2->f_name));
	return ft_strcmp_insensitive(f2->name, f1->name);
}