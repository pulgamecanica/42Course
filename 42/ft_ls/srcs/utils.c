#include "ft_ls.h"

static void add_to_total(void * ptr1, void * ptr2) {
	int * total;
	t_file * file;

	file = (t_file *)ptr1;
	total = (int *)ptr2;

	*total += file->stat.st_blocks / 2;
}

/**
 * get_total_block_size
 * Will iterate the list and add to the 
 * total to return the value.
 * Perfect example of C's beautiful abstraction.
 **/
int get_total_block_size(t_list * list) {
	int total;

	total = 0;
	ft_lstiter_param(list, add_to_total, &total);
	return (total);
}