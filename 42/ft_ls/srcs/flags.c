#include "conf.h"
#include "libft.h"
#include "ft_printf.h"

static bool add_short_options(char * str, t_conf * conf);

static void print_help(void) {
	ft_printf(""
"List information about the FILEs (the current directory by default).\n"
"Sort entries alphabetically if none of -cftuvSUX[REVIEW] nor --sort is specified.\n"
"\n"
"Mandatory arguments to long options are mandatory for short options too.\n"
"  -a, --all                  do not ignore entries starting with .\n"
"  -A, --almost-all           do not list implied . and ..\n"
"      --author               with -l, print the author of each file\n"
"\n"
"  -C                         list entries by columns\n"
"      --color[=WHEN]         color the output WHEN; more info below\n"
"  -d, --directory            list directories themselves, not their contents\n"
"  -f                         list all entries in directory order\n"
"\n"
"  -g                         like -l, but do not list owner\n"
"\n"
"  -G, --no-group             in a long listing, don't print group names\n"
"\n"
"  -i, --inode                print the index number of each file\n"
"  -k, --kibibytes            default to 1024-byte blocks for file system usage;\n"
"                             used only with -s and per directory totals\n"
"\n"
"  -l                         use a long listing format\n"
"\n"
"  -m                         fill width with a comma separated list of entries\n"
"  -n, --numeric-uid-gid      like -l, but list numeric user and group IDs\n"
"  -o                         like -l, but do not list group information\n"
"  -p, --indicator-style=slash\n"
"                             append / indicator to directories\n"
"\n"
"  -r, --reverse              reverse order while sorting\n"
"  -R, --recursive            list subdirectories recursively\n"
"  -s, --size                 print the allocated size of each file, in blocks\n"
"  -S                         sort by file size, largest first\n"
// "      --sort=WORD            sort by WORD instead of name: none (-U), size (-S),\n"
// "                             time (-t), version (-v), extension (-X), width\n"
"  -t                         sort by time, newest first; see --time\n"
"\n"
"  -U                         do not sort; list entries in directory order\n"
"  -x                         list entries by lines instead of by columns\n"
"  -X                         sort alphabetically by entry extension\n"
"  -Z,                        print any security context of each file\n"
"  -1                         list one file per line\n"
"      --help        display this help and exit\n"
"      --version     output version information and exit\n"
"\n"
"The WHEN argument defaults to 'always' and can also be 'auto' or 'never'.\n"
"\n"
"Using color to distinguish file types is disabled both by default and\n"
"with --color=never.  With --color=auto, ls emits color codes only when\n"
"standard output is connected to a terminal.  The LS_COLORS environment\n"
"variable can change the settings.  Use the dircolors(1) command to set it.\n"
"\n"
"Exit status:\n"
" 0  if OK,\n"
" 1  if minor problems (e.g., cannot access subdirectory),\n"
" 2  if serious trouble (e.g., cannot access command-line argument).\n"
"\n"
"GNU coreutils online help: [REVIEW]<https://www.gnu.org/software/coreutils/>\n"
"Full documentation [REVIEW]<https://www.gnu.org/software/coreutils/ls>\n"
"or available locally via: info '(coreutils) ls invocation'\n"
	);
}

static void print_version(void) {
ft_printf(""
"ft_ls (FT coreutils) 1.0\n"
"Copyright (C) 2023 Pulgamecanica.\n"
"\n"
"Written by Pulgamecanica.\n"
);
}

static bool add_long_option(char * str, t_conf * conf) {
	if (ft_strcmp(str, "--all") == 0) { 
		conf->almost_no_ignore = false;
		conf->no_ignore = true;
	} else if (ft_strcmp(str, "--almost-all") == 0) { 
		conf->no_ignore = false;
		conf->almost_no_ignore = true;
	} else if (ft_strcmp(str, "--color=always") == 0) { 
		conf->color = true;
	} else if (ft_strcmp(str, "--color=never") == 0) { 
		conf->color = false;
	} else if (ft_strcmp(str, "--directory") == 0) { 
		conf->no_explore = true;
	} else if (ft_strcmp(str, "--no-group") == 0) { 
		conf->l_opts.group = false;
	} else if (ft_strcmp(str, "--inode") == 0) { 
		conf->inode = true;
	// } else if (ft_strcmp(str, "--kibibytes") == 0) { 
	// 	conf->l_opts.bytes_per_block = 1024;
	} else if (ft_strcmp(str, "--numeric-uid-gid") == 0) { 
		add_short_options("-l", conf);
		conf->l_opts.numeric_uid_gid = true;
	} else if (ft_strcmp(str, "--indicator-style=slash") == 0) { 
		conf->print_indicator = true;
	} else if (ft_strcmp(str, "--reverse") == 0) { 
		conf->sort_rev = true;
	} else if (ft_strcmp(str, "--recursive") == 0) {
		conf->recursive = true; 
	} else if (ft_strcmp(str, "--size") == 0) { 
		conf->size = true; 
	// } else if (ft_strcmp(str, "--sort=WORD") == 0) { 
	// } else if (ft_strcmp(str, "--context") == 0) { 
	} else if (ft_strcmp(str, "--version") == 0) { 
		print_version();
		return (false);
	} else if (ft_strcmp(str, "--help") == 0) { 
		print_help();
		return (false);
	} else	{
		ft_printf(""
			"ls: unrecognized option '%s'\n"
			"Try 'ls --help' for more information.\n",
			str);
		return (false);
	}
	return (true);
}

static bool add_short_options(char * str, t_conf * conf) {
	int len;

	len = ft_strlen(str);
	for (int i = 1; i < len; i++)
	{
		if (str[i] == 'a') {
			conf->almost_no_ignore = false;
			conf->no_ignore = true;
		} else if (str[i] == 'A') {
			conf->no_ignore = false;
			conf->almost_no_ignore = true;
		} else if (str[i] == 'C') {
			conf->format = multiple_in_column;
		} else if (str[i] == 'd') {
			conf->no_explore = true;
		} else if (str[i] == 'f') {
			add_short_options("-a", conf);
			conf->sorting = by_directory;
		} else if (str[i] == 'g') {
			add_short_options("-l", conf);
			conf->l_opts.owner = false;
		} else if (str[i] == 'G') {
			conf->l_opts.group = false;
		} else if (str[i] == 'i') {
			conf->inode = true;
		// } else if (str[i] == 'k') {
		} else if (str[i] == 'l') {
			conf->format = long_format;
		} else if (str[i] == 'm') {
			conf->format = coma_separated;
		} else if (str[i] == 'n') {
			conf->l_opts.numeric_uid_gid = true;
		} else if (str[i] == 'o') {
			add_short_options("-l", conf);
			conf->l_opts.group = false;
		} else if (str[i] == 'p') {
			conf->print_indicator = true;
		} else if (str[i] == 'r') {
			conf->sort_rev = true;
		} else if (str[i] == 'R') {
			conf->recursive = true;
		} else if (str[i] == 's') {
			conf->size = true;
		} else if (str[i] == 'S') {
			conf->sorting = by_size;
		} else if (str[i] == 't') {
			conf->sorting = by_time;
		} else if (str[i] == 'U') {
			conf->sorting = no_sorting;
		} else if (str[i] == 'x') {
			conf->format = multiple_in_row;
		} else if (str[i] == 'X') {
			conf->sorting = by_ext;
		// } else if (str[i] == 'Z') {
		// 	conf->recursive = true;
		} else if (str[i] == '1') {
			if (!(conf->format == long_format))
				conf->format = one_per_line;
		} else if (str[i] == 'r') {
			conf->recursive = true;
		} else {
			ft_printf(""
				"ls: invalid option -- '%c'\n"
				"Try 'ls --help' for more information.\n",
				str[i]);
			return (false);
		}
	}
	return (true);
}

bool add_flag(char * str, t_conf * conf) {
	// This function will only be called after is_flag
	if (!conf || !str)
		return (false);

	if (*(str + 1) == '-')
	{
		if (*(str + 2) == 0) {
			// when str is equal to '--' 
			conf->delimit = true;
			return (true);
		} else {
			return (add_long_option(str, conf));
		}
	}
	return (add_short_options(str, conf));
}

bool is_flag(char * str) {
	// str is not null and str beggins with '-' and str has more than one character
	return (str && *str == '-' && *(str + 1) != 0);
}