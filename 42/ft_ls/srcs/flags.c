#include "ft_ls.h"


static bool is_long_option_flag(char *str) {
	return ft_strncmp(str, "--", 2) == 0;
}

static bool add_long_option_flag(t_conf * conf, char * str) {
	if (ft_strcmp(str, "--all") == 0) {
		conf->no_ignore = true;
	}if (ft_strcmp(str, "--author") == 0) {
		conf->print_author = true;
	}  else if (ft_strcmp(str, "--recursive") == 0) {
		conf->recursive = true;
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

bool add_flags(t_conf * conf, char * str) {
	if (ft_strcmp(str, "--") == 0) {
		return conf->delimit = true;
	}
	if (is_long_option_flag(str))
		return (add_long_option_flag(conf, str));
	int i = 0;
	while (str[++i]) {
		if (!add_flag(conf, str[i]))
			return (false);
	}
	return (true);
}
