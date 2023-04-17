#include "ft_ls.h"
#define default_col_width 3
#define min_cols 3

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

static size_t get_max_arg_file_name(ls_config * conf) {
	size_t max_width_file;
	t_list * tmp;

	if (!conf || !conf->files)
		return -1;
	max_width_file = 3;
	tmp = conf->files;
	while(tmp) {
		max_width_file = ft_max(max_width_file, ft_strlen(((t_file *)(tmp->content))->f_name));
		tmp = tmp->next;
	}
	return max_width_file;
}

void	ft_print_regular_files(ls_config * conf) {
	char * file_padding;
	char *	file_time;
	char *	format;
	char *	tmp_str;
	t_list * tmp;
	t_file * file;

	if (!conf->files)
		ft_exit(1, "Bad Error\n", 0);
	file_padding = ft_itoa(get_max_arg_file_name(conf));
	format = ft_strdup("%-");
	format = ft_strjoin(format, file_padding);
	format = ft_strjoin(format, "s");
	free(file_padding);
	tmp = conf->files;
	while (tmp) {
		file = (t_file *)tmp->content;
		if (file->f_errors == NotFounded) {
			ft_printf("ls: cannot access '%s': No such file or directory\n", file->f_name);
		} else if (file->f_type != Directory || conf->flags.flagd) {
			if (conf->flags.flagl) {
				file_time = get_file_time(file, &conf->flags);
				ft_printf(""
					"%.0d"/*inode number [only print when -i is present]*/
					"%.0d"/*file sistem allocation space [only print when -s is present]*/
					"%c"/*file type*/
					"%c"/*file mode bit-1*/
					"%c"/*file mode bit-2*/
					"%c"/*file mode bit-3*/
					"%c"/*file mode bit-4*/
					"%c"/*file mode bit-5*/
					"%c"/*file mode bit-6*/
					"%c"/*file mode bit-7*/
					"%c"/*file mode bit-8*/
					"%c"/*file mode bit-9*/
					" %d"/*number of hard links*/
					" %s"/*owner name [-g should omit owner information] [-n print right justified id numbers instead of names]*/
					" %s"/*group name [-G should omit the group display	]*/
					"  %10d"/*size, power of 1024 but could be changed to power of 1000 with flag --si*/
					" %s"/*timestamp [default last time modification]*/
					" %s"/*file name*/
					"%s"/*" -> " when soft link*/
					"%s"/*soft link pointed file name*/
					"\n"
					, 0/*inode number*/
					, 0/*file sistem allocation space*/
					, file->f_type/*file type*/
					, 45/*file mode bit-1*/
					, 45/*file mode bit-2*/
					, 45/*file mode bit-3*/
					, 45/*file mode bit-4*/
					, 45/*file mode bit-5*/
					, 45/*file mode bit-6*/
					, 45/*file mode bit-7*/
					, 45/*file mode bit-8*/
					, 45/*file mode bit-9*/
					, file->f_stat.st_nlink/*number of hard links*/
					, "pulgamecanica"/*owner name*/
					, "pulgamecanica"/*group name*/
					, file->f_stat.st_size/*size*/
					, file_time/*timestamp*/
					, file->f_name/*file name*/
					, (file->f_type == Symbolic_Link) ? " -> " : ""/*" -> " when soft link*/
					, (file->f_type == Symbolic_Link) ? file->f_link_file->f_name : ""/*soft link pointed file name*/
					);
				free(file_time);
			} else {
				//format = horizontal_format(conf->files, conf);
				tmp_str = ft_strdup(file->f_name);
				if (conf->flags.flagp)
					if (file->f_type == Directory)
						tmp_str = ft_strjoin(tmp_str, "/");
				if (conf->flags.flagm) {
					tmp_str = ft_strjoin(tmp_str, ",");
					ft_printf ("%s", tmp_str);
				} else {
					ft_printf (format, tmp_str);
				}
				write(1, " ", 1);
				free(tmp_str);
			}
		}
		tmp = tmp->next;
	}
	free(format);
	write(1, "\n", 1);
}






























































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

char *	get_horizontal_format_tmp(t_file * parent, ls_config * conf) {
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
		str_format = get_horizontal_format_tmp(parent_dir, conf);
		//cols = get_number_cols(parent_dir, conf);
		ft_printf(str_format, "HEY!");
		free(str_format);
//	} else if (str_format == long_format) {

//	}
}
