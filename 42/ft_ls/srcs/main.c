#include "ft_ls.h"

/*
#include "libft.h"
#include "ft_printf.h"
#include <stdbool.h>

typedef struct ls_flags {
	bool flagl; // -l
	bool flagR; // -R --recursive
	bool flaga; // -a --all
	bool flagr; // -r --reverse
	bool flagt; // -t
}	ls_flags;
*/


void	ft_exit(int status, char * msg, bool msg_allocated) {
	write(2, msg, ft_strlen(msg));
	if (msg_allocated)
		free(msg);
	exit(status);
}

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
int	is_flag(char * str) {
	if (ft_strlen(str) < 2)
		return 0;
	if (str[0] == '-' && str[1] != '-')
		return 1;
	else if (str[0] == '-' && str[1] == '-')
		return 2;
	return 0;
}

void	set_flag(ls_flags * flags, char flag) {
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
 **/
void	set_full_flag(ls_flags * flags, char * str) {
	char * error_msg;

	if (ft_strncmp("--recursive", str, ft_max(ft_strlen(str), ft_strlen("--recursive"))) == 0)
		set_flag(flags, 'R');
	else if (ft_strncmp("--all", str, ft_max(ft_strlen(str), ft_strlen("--all"))) == 0)
		set_flag(flags, 'a');
	else if (ft_strncmp("--reverse", str, ft_max(ft_strlen(str), ft_strlen("--reverse"))) == 0)
		set_flag(flags, 'r');
	else if (ft_strlen(str) != 2) {
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
	if (!flag_type)
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

int	main(int ac, char * av[]) {
	ls_flags flags;

	for (int i = 1; i < ac; i++) {
		assign_flags(&flags, av[i]);
	}
	return 1;
}
