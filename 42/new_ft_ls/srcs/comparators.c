#include "ft_ls.h"

/**
 * The comparators take advantage on the abstraction of the linked list
 * The comparators are used to sort the linked list
 * The merge algorithm sorts the elements based on the function you pass to them
 * A reverse comparator can use the same function but returning the oposite (or multiply by -1)
 * 
 * Example:
 * sort_by_name();
 * 
 * sort_by_name_reverse() {
 * 	return (sort_by_name() * -1);
 * }
 * */
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