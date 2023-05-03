#include "ft_ls.h"


static bool is_long_option_flag(char *str) {
	return ft_strncmp(str, "--", 2) == 0;
}

static bool add_long_option_flag(t_conf * conf, char * str) {
	if (ft_strcmp(str, "--all") == 0) {
		conf->no_ignore = true;
	} /* else if () {
	} */
	 else {
	 	return (false);
	}
	return (true);
}

static bool add_flag(t_conf * conf, char flag) {
	if (flag == 'a') {
		conf->no_ignore = true;
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
