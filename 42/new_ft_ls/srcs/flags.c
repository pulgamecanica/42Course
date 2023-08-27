#include "ft_ls.h"

static bool add_long_option_flag(t_conf * conf, char * str) {
	if (ft_strcmp(str, "--all") == 0) {
		conf->no_ignore = true;
	} if (ft_strcmp(str, "--author") == 0) {
		conf->print_author = true;
	} else if (ft_strcmp(str, "--recursive") == 0) {
		conf->recursive = true;
	} else if (ft_strcmp(str, "--help") == 0) {
		ft_printf(""
			"Usage: ls [OPTION]... [FILE]...\n"
			"List information about the FILEs (the current directory by default).\n"
			"Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.\n"
			"\n"
			"Mandatory arguments to long options are mandatory for short options too.\n"
			"\t-a, --all                  do not ignore entries starting with .\n"
			"\t-d, --directory            list directories themselves, not their contents\n"
			"\t-g                         like -l, but do not list owner\n"
			"\t-G, --no-group             in a long listing, don't print group names\n"
			"\t-i, --inode                print the index number of each file\n"
			"\t-l                         use a long listing format\n"
			"\t-m                         fill width with a comma separated list of entries\n"
			"\t-o                         like -l, but do not list group information\n"
			"\t-R, --recursive            list subdirectories recursively\n"
			"\t-s, --size                 print the allocated size of each file, in blocks\n"
			"\t-x                         list entries by lines instead of by columns\n"
			"\t-1                         list one file per line.  Avoid '\\n' with -q or -b\n"
      		"\t--help     display this help and exit\n"
		);
	 	return (false);
	} /* else if () {
	} */
	 else {
		ft_printf(""
			"ls: unrecognized option '%s'\n"
			"Try 'ls --help' for more information.\n", str);
	 	return (false);
	}
	return (true);
}

static bool add_flag(t_conf * conf, char flag) {
	if (flag == 'a') {
		conf->no_ignore = true;
	} else if (flag == 'i') {
		conf->print_inode = true;
	} else if (flag == 's') {
		conf->print_block_size = true;
	} else if (flag == 'g') {
		add_flag(conf, 'l');
		conf->print_owner = false;
	} else if (flag == 'G') {
		conf->print_group = false;
	} else if (flag == '1') {
		conf->format = OnePerLine;
	} else if (flag == 'm') {
		conf->format = ComaSeparated;
	} else if (flag == 'x') {
		conf->format = MultipleColumn;
	} else if (flag == 'l') {
		conf->format = LongFormat;
	} else if (flag == 'o') {
		add_flag(conf, 'l');
		add_flag(conf, 'G');
	} else if (flag == 'R') {
		conf->recursive = true;
	} else if (flag == 'd') {
		conf->no_explore = true;
	} else {
		ft_printf(""
			"ls: invalid option -- '%c'\n"
			"Try 'ls --help' for more information.\n", flag);
		return (false);
	}
	return (true);
}

/**
 * Add the flags
 * Flags can be in long format (--)
 * Flags can be in normal format (-)
 **/
bool add_flags(t_conf * conf, char * str) {
	int i;

	if (ft_strcmp(str, "--") == 0)
		return (conf->delimit = true);
	if (ft_strncmp(str, "--", 2) == 0)
		return (add_long_option_flag(conf, str));
	i = 0;
	while (str[++i])
		if (!add_flag(conf, str[i]))
			return (false);
	return (true);
}
