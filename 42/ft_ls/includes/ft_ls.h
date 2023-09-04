#ifndef __FT_LS__
#define __FT_LS__

void *	sorting_func(enum sorting s, bool rev);
void *	format_func(enum format f);
bool	is_flag(char * str);
bool	add_flag(char * str, t_conf * conf);
void	list_directory(void * ptr1, void * ptr2);
#endif