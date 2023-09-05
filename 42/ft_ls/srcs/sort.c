#include "file.h"
#include "libft.h"

// I believe that ls only sorts by ALPHA NUMERIC 
// ALL other symbols are ignored
// Must create a strcmp_alpha_only function!

// ONLY THING MISSING :D 
// Tomorrow I will be testing ALL the options to deliver the project!? :D
static int sort_by_name(void * ptr1, void * ptr2) {
	t_file * f1, * f2;

	f1 = (t_file *)ptr1;
	f2 = (t_file *)ptr2;
	if (!f1 && !f2)
		return (0);
	if (!f2)
		return (1);
	if (!f1)
		return (-1);
	return (ft_strcmp_insensitive(f2->name, f1->name));
}

static int sort_by_name_rev(void * ptr1, void * ptr2) {
	return (sort_by_name(ptr2, ptr1));
}

static int sort_by_ext(void * ptr1, void * ptr2) {
	t_file * f1, * f2;
	char * ext1, * ext2;

	f1 = (t_file *)ptr1;
	f2 = (t_file *)ptr2;
	if (!f1 && !f2)
		return (0);
	ext1 = ft_strchr(f1->name, '.');
	ext2 = ft_strchr(f2->name, '.');
	if (ext1 && ext2)
		return (ft_strcmp_insensitive(ext2, ext1));
	if (ext2)
		return (1);
	return sort_by_name(ptr1, ptr2);
}

static int sort_by_ext_rev(void * ptr1, void * ptr2) {
	t_file * f1, * f2;
	char * ext1, * ext2;

	f1 = (t_file *)ptr1;
	f2 = (t_file *)ptr2;
	if (!f1 && !f2)
		return (0);
	ext1 = ft_strchr(f1->name, '.');
	ext2 = ft_strchr(f2->name, '.');
	if (ext1 && ext2)
		return (ft_strcmp_insensitive(ext1, ext2));
	if (ext2)
		return (-1);
	return sort_by_name(ptr2, ptr1);
}

static int sort_by_time_last_mod(void * ptr1, void * ptr2) {
	t_file * f1, * f2;

	f1 = (t_file *)ptr1;
	f2 = (t_file *)ptr2;
	if (!f1 && !f2)
		return (0);
	return (f1->stat.st_mtime - f2->stat.st_mtime);
}

static int sort_by_time_last_mod_rev(void * ptr1, void * ptr2) {
	return (sort_by_time_last_mod(ptr2, ptr1));
}

static int sort_by_size(void * ptr1, void * ptr2) {
	t_file * f1, * f2;

	f1 = (t_file *)ptr1;
	f2 = (t_file *)ptr2;
	if (!f1 && !f2)
		return (0);
	if (!f2)
		return (1);
	if (!f1)
		return (-1);
	return (f1->stat.st_size - f2->stat.st_size);
}

static int sort_by_size_rev(void * ptr1, void * ptr2) {
	return (sort_by_size(ptr2, ptr1));
}

void * sorting_func(enum sorting s, bool rev) {
	switch (s) {
		case by_name: return (rev ? sort_by_name_rev : sort_by_name);
		case by_size: return (rev ? sort_by_size_rev : sort_by_size);
		case by_time: return (rev ? sort_by_time_last_mod_rev : sort_by_time_last_mod);
		case by_ext: return (rev ? sort_by_ext_rev : sort_by_ext);
		case no_sorting: return (NULL);
		default: return (sort_by_name);
	}
}
