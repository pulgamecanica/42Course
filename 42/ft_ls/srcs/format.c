#include "file.h"
#include "ft_printf.h"

static void one_per_line_format_f(void * ptr) {
	t_file * f;

	f = (t_file *)ptr;
	if (!f)
		return ;
	ft_printf("%s\n", f->name);
}

static void long_format_f(void * ptr) {
	t_file * f;

	f = (t_file *)ptr;
	if (!f)
		return ;
	ft_printf("long format -> %s\n", f->name);
}

void * format_func(enum format f) {
	switch (f) {
		case one_per_line: return one_per_line_format_f;
		case multiple_in_column: return one_per_line_format_f;
		case multiple_in_row: return one_per_line_format_f;
		case long_format: return long_format_f;
		case coma_separated: return one_per_line_format_f;
		default: return one_per_line_format_f;
	}
}