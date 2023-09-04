#include "file.h"
#include "libft.h"

// I believe that ls only sorts by ALPHA NUMERIC 
// ALL other symbols are ignored
// Must create a strcmp_alpha_only function!
int sort_by_name(void * ptr1, void * ptr2) {
	t_file * f1, * f2;

	f1 = (t_file *)ptr1;
	f2 = (t_file *)ptr2;
	if (!f1 && !f2)
		return 0;
	if (!f2)
		return 1;
	if (!f1)
		return -1;
	return (ft_strcmp_insensitive(f2->name, f1->name));
}

int sort_by_name_rev(void * ptr1, void * ptr2) {
	return (sort_by_name(ptr2, ptr1));
}

int sort_by_size(void * ptr1, void * ptr2) {
	t_file * f1, * f2;

	f1 = (t_file *)ptr1;
	f2 = (t_file *)ptr2;
	if (!f1 && !f2)
		return 0;
	if (!f2)
		return 1;
	if (!f1)
		return -1;
	return (f1->stat.st_size - f2->stat.st_size);
}

int sort_by_size_rev(void * ptr1, void * ptr2) {
	return (sort_by_size(ptr2, ptr1));
}

void * sorting_func(enum sorting s, bool rev) {
	switch (s) {
		case by_name: return (rev ? sort_by_name_rev : sort_by_name);
		case by_size: return (rev ? sort_by_size_rev : sort_by_size);
		default: return sort_by_name;
	}
}