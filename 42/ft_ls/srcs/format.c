#include "ft_ls.h"

int	get_max_file_name(t_file * parent) {
	size_t max_width_file;
	t_list * tmp;

	if (!parent || !parent->children)
		return -1;
	max_width_file = 3;
	tmp = parent->children;
	while(tmp) {
		max_width_file = ft_max(max_width_file, ft_strlen(((t_file *)(tmp->content))->f_name));
		tmp = tmp->next;
	}
	return max_width_file;
}

/** when flag -F then
 * %s%c
 * s is the name of the file
 * c is the file type
 * executables append *
 * directories append /
 * symbolic links append @
 * fifos append |
 * sockets append =
 * doors append ?
 * regular files, nothing
 **/
/** when flag -m then
 * %s,
 * s is the name of the file
 * , is the separator
 **/
/** when flag -p then
 * %s%s
 * append a slash for directories 
 **/

/**
 * Returns the format that one file will use to display informations
**/
char	* horizontal_format(t_file * parent, ls_config * config) {
	char *	file_padding;
	char *	format;

	if (!parent || !config)
		return NULL;
	file_padding = ft_itoa(get_max_file_name(parent));
	format = ft_strdup("%");
	format = ft_strjoin(format, file_padding);
	format = ft_strjoin(format, "s");
	if (config->flags.flagm && config->flags.flagp) {
		format = ft_strjoin(format, "%.1s%.1s");
	} else if (config->flags.flagp || config->flags.flagm) {
		format = ft_strjoin(format, "%.1s");
	}
	free(file_padding);
	return format;
}

char	* multiple_column_format(t_file * parent, ls_config * config) {
	return horizontal_format(parent, config);
}

char	* long_format_format(t_file * parent, ls_config * config) {
//	get_max_file_name
	(void)parent;
	if (config->flags.flagp)
		return ft_strdup(""
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
		"%.1s"/* -m option, print '/' when dir*/
		);
	else
		return ft_strdup(""
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
		);
}
char	* one_per_line_format(t_file * parent, ls_config * config) {
	char *	file_padding;
	char *	format;

	if (!parent || !config)
		return NULL;
	file_padding = ft_itoa(get_max_file_name(parent));
	format = ft_strdup("%");
	format = ft_strjoin(format, file_padding);
	format = ft_strjoin(format, "s");
	if (config->flags.flagp)
		format = ft_strjoin(format, "%.1s");
	free(file_padding);
	return format;
}



/*
char * get_file_time(t_file * file, ls_flags * flags) {
	char * time_str;
	char * str;

	if (!flags || !file || file->f_errors != NoError)
		return NULL;
	time_str = NULL;
	if (flags->flagt) {
		time_str = ctime(&file->f_stat.st_mtime);
	} else {
		time_str = ctime(&file->f_stat.st_ctime);
	}
	str = (char *)ft_calloc(sizeof(char), ft_strlen(time_str));
	if (!str)
		return NULL;
	ft_strlcpy(str, time_str, ft_strlen(time_str) - 1);
	return str;
}
*/