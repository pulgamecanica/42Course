#ifndef __FT_LS__
#define __FT_LS__

void *	sorting_func(enum sorting s, bool rev);
void *	format_func(enum format f);
bool	is_flag(char * str);
bool	add_flag(char * str, t_conf * conf);
void	list_directory(void * file, void * conf);
void	setup_padding(void * file, void * conf);
void	setup_total(void * file, void * number);
void	setup_six_months_from_now(t_conf * conf);
int		ft_ceil(float num);

#endif
