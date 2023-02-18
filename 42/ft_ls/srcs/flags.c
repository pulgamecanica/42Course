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
	case 'l':
		flags->flagl = true;
		break ;
	case 'R':
		flags->flagR = true;
		break ;
	case 'a':
		flags->flaga = true;
		break ;
	case 'r':
		flags->flagr = true;
		break ;
	case 't':
		flags->flagt = true;
		break ;
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

	if (ft_strncmp("--recursive", str, ft_max(ft_strlen(str), ft_strlen("--recursive"))) == 0)
		set_flag(flags, 'R');
	else if (ft_strncmp("--all", str, ft_max(ft_strlen(str), ft_strlen("--all"))) == 0)
		set_flag(flags, 'a');
	else if (ft_strncmp("--reverse", str, ft_max(ft_strlen(str), ft_strlen("--reverse"))) == 0)
		set_flag(flags, 'r');
	else if (ft_strncmp("--", str, ft_max(ft_strlen(str), ft_strlen("--"))) == 0) {
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
		" -l %s\n"
		" -R %s\n"
		" -a %s\n"
		" -r %s\n"
		" -t %s\n"
		"delimit %s\n",
		flags->flagl ? "true" : "false",
		flags->flagR ? "true" : "false",
		flags->flaga ? "true" : "false",
		flags->flagr ? "true" : "false",
		flags->flagt ? "true" : "false",
		flags->delimit ? "true" : "false"
	);
}
