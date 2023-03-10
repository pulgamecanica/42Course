#include "ft_ls.h"

/**
 * Returns 0 if it's no flag
 * Returns 1 if it's a simple flag (-)
 * Returns 2 if is's a full flag (--)
 * If the string is only "-" it's concidered
 * as a file instead of flag.
 * If the string is "--" it's concidered as a full
 * flag, and full flag can be empty... apparently
 * But still need to check that it's not a wrong flag.
 * ERROR
 * simple:
 *  ls: invalid option -- '0'
 *  Try 'ls --help' for more information.
 * full:
 *  ls: unrecognized option '--asfas'
 *  Try 'ls --help' for more information.
 **/
static int	is_flag(char * str) {
	if (ft_strlen(str) < 2)
		return 0;
	if (str[0] == '-' && str[1] != '-')
		return 1;
	else if (str[0] == '-' && str[1] == '-')
		return 2;
	return 0;
}

static void	set_flag(ls_flags * flags, char flag) {
	char tmp[] = "ls: invalid option -- 'c'\nTry 'ls --help' for more information.\n";
	switch (flag) {
	case 'l': flags->flagl = true; break ;
	case 'R': flags->flagR = true; break ;
	case 'a': flags->flaga = true; break ;
	case 'r': flags->flagr = true; break ;
	case 't': flags->flagt = true; break ;
	case 'L': flags->flagL = true; break ;
	case 'D': flags->flagD = true; break ;
	case 'u': flags->flagu = true; break ;
	case 'f': flags->flagf = true; break ;
	case 'g': flags->flagg = true; break ;
	case 'd': flags->flagd = true; break ;
	case '1': flags->flag1 = true; break ;
	case 'p': flags->flagp = true; break ;
	case 'm': flags->flagm = true; break ;
	case 'x': flags->flagx = true; break ;
	case 'i': flags->flagi = true; break ;
	case 'C': flags->flagC = true; break ;
	default:
		tmp[23] = flag;
		ft_exit(2, tmp, false);
	}
}

/**
 * Returns 1 on success
 * Returns 0 on error
 * str ALWAYS starts with "--"
 * if str is "--", then delimit flag is activated
 **/
static void	set_full_flag(ls_flags * flags, char * str) {
	char *	error_msg;

	if (ft_strncmp("--recursive", str, ft_max(ft_strlen(str), 11)) == 0)
		set_flag(flags, 'R');
	else if (ft_strncmp("--all", str, ft_max(ft_strlen(str), 4)) == 0)
		set_flag(flags, 'a');
	else if (ft_strncmp("--reverse", str, ft_max(ft_strlen(str), 9)) == 0)
		set_flag(flags, 'r');
	else if (ft_strncmp("--", str, ft_max(ft_strlen(str), 2)) == 0) {
		flags->delimit = true;
	} else {
		error_msg = NULL;
		if (!(error_msg = ft_strjoin(error_msg, "ls: unrecognized option '")))
			ft_exit(2, "Fatal Memory Could Not Be Allocated\n", false);
		if (!(error_msg = ft_strjoin(error_msg, str)))
			ft_exit(2, "Fatal Memory Could Not Be Allocated\n", false);
		if (!(error_msg = ft_strjoin(error_msg, "'\nTry 'ls --help' for more information.\n")))
			ft_exit(2, "Fatal Memory Could Not Be Allocated\n", false);
		ft_exit(2, error_msg, true);
	}
}

/**
 * Returns 1 on flag assigned
 * Returns 0 on no flag assigned
 **/
int	assign_flags(ls_flags * flags, char * str) {
	size_t	i, flag_type;

	flag_type = is_flag(str);
	if (!flag_type || flags->delimit)
		return 0;
	if (flag_type == 2) {
		set_full_flag(flags, str);
	} else if (flag_type == 1) {
		i = 1;
		while(i < ft_strlen(str)) {
			set_flag(flags, str[i]);
			i++;
		}
	}
	return 1;
}

void print_flags(ls_flags * flags) {
	if (!flags)
		return ;
	ft_printf("Flags\n"
		" -l [%- 15.15s] --> %s\n"
		" -R [%- 15.15s] --> %s\n"
		" -a [%- 15.15s] --> %s\n"
		" -r [%- 15.15s] --> %s\n"
		" -t [%- 15.15s] --> %s\n"
		" -L [%- 15.15s] --> %s\n"
		" -D [%- 15.15s] --> %s\n"
		" -u [%- 15.15s] --> %s\n"
		" -f [%- 15.15s] --> %s\n"
		" -g [%- 15.15s] --> %s\n"
		" -d [%- 15.15s] --> %s\n"
		" -1 [%- 15.15s] --> %s\n"
		" -p [%- 15.15s] --> %s\n"
		" -m [%- 15.15s] --> %s\n"
		" -x [%- 15.15s] --> %s\n"
		" -i [%- 15.15s] --> %s\n"
		" -C [%- 15.15s] --> %s\n"
		"delimit %s\n",
		"long format", flags->flagl ? "true" : "false",
		"recursive", flags->flagR ? "true" : "false",
		"almost all", flags->flaga ? "true" : "false",
		"sort reverse", flags->flagr ? "true" : "false",
		"sort time", flags->flagt ? "true" : "false",
		"show file no link", flags->flagL ? "true" : "false",
		"show dired", flags->flagD ? "true" : "false",
		"show create timestamp", flags->flagu ? "true" : "false",
		"unsorted -1 on", flags->flagf ? "true" : "false",
		"long format no owner", flags->flagg ? "true" : "false",
		"show dir no listing", flags->flagd ? "true" : "false",
		"one row per file", flags->flag1 ? "true" : "false",
		"show '/' for dirs", flags->flagp ? "true" : "false",
		"coma separator", flags->flagm ? "true" : "false",
		"show file in cols", flags->flagx ? "true" : "false",
		"show innode number", flags->flagi ? "true" : "false",
		"list with columns", flags->flagC ? "true" : "false",
		flags->delimit ? "true" : "false"
	);
}
