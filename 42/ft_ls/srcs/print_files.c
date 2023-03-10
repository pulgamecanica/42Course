#include "ft_ls.h"

/**
 * Given the directory arguments and concidering the flags, format the output accordingly
 * By default the listing is multiple_column formating
 * -l long_format
 * -1 one file per line one_per_line
 * -x horizontal
	EX:
	[multiple_column]
	a	d	x
	b	e	z
	c	f
	[long_format]
	type ... time a
	type ... time b
	type ... time z
	[one_per_line]
	a
	b
	z
	[horizontal]
	a	b	c
	d	e	f
	y	z
 **/
#define default_col_width

int	get_number_cols(t_file * parent, ls_config * conf) {
	size_t max_width_file;
	int columns;
	t_list * tmp;

	if (!parent || !conf || !parent->children)
		return -1;
	max_width_file = 3;
	tmp = parent->children;
	while(tmp) {
		max_width_file = ft_max(max_width_file, ft_strlen(((t_file *)(tmp->content))->f_name));
		tmp = tmp->next;
	}
	columns = conf->sz.ws_col / max_width_file;
	if (columns <= 0)
		columns = 1;
	return columns;
}

char *	get_horizontal_format(t_file * parent, ls_config * conf) {
	char * format;
	char * num_tmp, * str_tmp;
	int columns;

	columns = get_number_cols(parent, conf);
	num_tmp = ft_itoa(columns);
	format = NULL;
	while (columns > 0) {
		str_tmp = ft_strdup("%");
		str_tmp = ft_strjoin(str_tmp, num_tmp);
		str_tmp = ft_strjoin(str_tmp, "s");
		if (conf->flags.flagp)
			str_tmp = ft_strjoin(str_tmp, "%s");
		if (conf->flags.flagm)
			str_tmp = ft_strjoin(str_tmp, ",");
		str_tmp = ft_strjoin(str_tmp, " ");
		format = ft_strjoin(format, str_tmp);
		columns--;
	}
	ft_printf("WINDOW: [%ux%u]\tcols: [%s]\n", conf->sz.ws_col, conf->sz.ws_row, num_tmp);
	free(num_tmp);
	return format;
}


void	print_all_children(/*enum format format,*/ void * ptr1, void * ptr2) {
	char * str_format;
	t_file * parent_dir;
	ls_config * conf;

	parent_dir = (t_file *)ptr1;
	conf = (ls_config *)ptr2;
	//int cols;

	if (!parent_dir || !conf)
		return ;
	//if (format == horizontal) {
		str_format = get_horizontal_format(parent_dir, conf);
		//cols = get_number_cols(parent_dir, conf);
		ft_printf(str_format, "HEY!");
		free(str_format);
//	} else if (str_format == long_format) {

//	}
}
