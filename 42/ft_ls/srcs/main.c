#include "ft_ls.h"
/******************************\
 *  __  _            _         *
 *  / _|| |          | |       *
 * | |_ | |_         | | ___   *
 * |  _|| __|        | |/ __|  *
 * | |  | |_         | |\__ \  *
 * |_|   \__|        |_||___/  *
 *            ______           *
 *           |______|          *
 \*****************************/
/***********************************************************************************************************************\
 * ls - list directory contents                                                                                        *
 * ls [OPTION]... [FILE]...                                                                                            *
 *                                                                                                                     *
 * List information about the FILEs (default would be '.'/ working directory)                                          *
 * Sort all enrties alphabetically, if nono of -cftuvSUX nor --sort specified.                                         *
 *                                                                                                                     *
 * Mandatory arguments to long options are mandatory for short options too (not applied for this implementation)       *
 *                                                                                                                     *
 *                   Description                          Implemented    Activates        Deactivates     Excludes     *
 * ----------------------------------------------------- * --------- * --------------- * -------------- * ---------- * *
 * -a, --all                                             |    YES    |                 |                |            | *
 *         do not ignore entries that start with .       |    :)     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -A, --almost-all                                      |    YES    |                 |                |            | *
 *         do not list . and ..                          |    :)     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * --author                                              |    NO     |                 |                |            | *
 *         with -l, print the author of each file        |    :(     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -b, --escape                                          |    NO     |                 |                |            | *
 *         print C-style escape characters               |    :(     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 *  -C                                                   |    YES    |                 |                |            | *
 *         list entries by columns                       |     :)    |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -d, --directory                                       |    YES    |                 |                |            | *
 *         list directories, not their content           |    :)     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -F, --classify[=WHEN]                                 |    YES    |                 |                |            | *
 *         appnd indicator (/=>*@|)                      |    :)     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -g  like -l but omit the owner                        |    YES    |                 |                |            | *
 * -G when -l omit the group                             |    YES    |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -h, --human-readable                                  |    YES    |                 |                |            | *
 *         with -l and -s print sizes K, M, G            |    :)     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -i, --inode                                           |    YES    |                 |                |            | *
 *         print inode before file                       |    :)     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -l use a long listing format                          |    YES    |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -m fill width with a coma separated list              |    YES    |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -n, --numeric-uid-gid                                 |    NO     |                 |                |            | *
 *         like -l,, but list numeric use and group      |    :(     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -o like -l, but do not list group information         |    YES    |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -p, --indicator-style=slash                           |    YES    |                 |                |            | *
 *         append / indicator in directories             |    :)     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -r, --reverse                                         |    YES    |                 |                |            | *
 *         reverse order while sorting                   |    :)     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -R, --recursive                                       |    YES    |                 |                |            | *
 *         list subdirectories recursively               |    :)     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -s, --size                                            |    NO     |                 |                |            | *
 *         print the allocated size of each file         |    :(     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 *  -t sort by time, newest first                        |    YES    |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 *  -u                                                   |    YES    |                 |                |            | *
 *         with -lt: sort by, and show, access time;     |    :)     |                 |                |            | *
 *         with -l: show access time and sort by name;   |           |                 |                |            | *
 *         otherwise: sort by access time, newest first  |           |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -U do not sort; list entries in directory order       |    YES    |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -v     natural sort of (version) numbers within text  |    NO     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -w, --width=COLS                                      |    NO     |                 |                |            | *
 *         set output width to COLS.  0 means no limit   |    :(     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -x list entries by lines instead of by columns        |    YES    |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -X sort alphabetically by entry extension             |    YES?   |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -Z, --context                                         |    NO     |                 |                |            | *
 *         print any security context of each file       |    :(     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * --zero end each output line with NUL, not newline     |   MAYBE   |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * -1     list one file per line                         |    YES    |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * --help display this help and exit                     |    YES    |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
 * --version                                             |    YES    |                 |                |            | *
 *         output version information and exit           |    :)     |                 |                |            | *
 * ------------------------------------------------------ ----------- ----------------- ---------------- ----------- * *
\************************************************************************************************************************/


static ls_config	init_config() {
	ls_config	config;

	ft_bzero(&config.flags, sizeof(ls_flags));
	config.files = NULL;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &config.sz);
	//int limit = config.sz.ws_row * config.sz.ws_col;
	//for (int i = 0; i < limit; i++)
	//	putchar('*');
	return config;
}

void print_list(t_list *lst, char * str) {
	t_list * tmp;

	if (!DEBUG)
		return ;
	ft_putendl_fd(str, 1);
	tmp = lst;
	while (tmp) {
		t_file * f;
		f = (t_file *)tmp->content;
		printf("\t%s\n", f->f_name);
		tmp = tmp->next;
	}
}

void	print_all_children(/*enum format format,*/ void * ptr1, void * ptr2);

int	main(int ac, char * av[]) {
	ls_config	config;

	config = init_config();
	for (int i = 1; i < ac; i++)
		if (!assign_flags(&config.flags, av[i]))
			ft_lstadd_front(&config.files, ft_lstnew(init_file(av[i], NULL)));
	if (!config.files || ft_lstsize(config.files) == 0)
		ft_lstadd_front(&config.files, ft_lstnew(init_file(".", NULL)));
	/**
	 * What Information is Listed?
	 * set up all the files, recursively if needed, soft links as well
	 * set file info, type and children or links.
	 **/
	ft_lstiter_param(config.files, setup_file, &config.flags);
	/**
	 * Sort the Arguments
	 * The set up files, should already be in order, because are added in order
	 **/
	print_list(config.files, "Args Before");
	// ORDER DIRECTORIES JUST LIKE FILENAMES
	// JUST OMIT INFORMATION WHEN NOT -d
	if (config.flags.flagr) {
		ft_lstsort(&config.files, cmp_rev_not_exist_first);
	} else {
		ft_lstsort(&config.files, cmp_not_exist_first);
	}
	print_list(config.files, "Args After");
	/**
	 * Print the output to general formatig, by default by columns
	 * Separating files by an empty line before, and the file name on top
	 * Unless it's only the file as argument, then only list file content
	 * First print the arguments, and in case the argument is a directory, list the files of the directory
	 **/
	ft_print_regular_files(&config);

	ft_lstiter_param(config.files, ft_print_directory_files, &config.flags);



	//ft_lstiter_param(config.files, print_all_children, &config);
	// ft_print_regular_files(config.files,)
	//ft_print_files(config.files, 0, ft_lstsize(config.files) > 1);
	//	Print debug info here
	if (DEBUG) {
		print_flags(&config.flags);
	}
	ft_lstclear(&config.files, free_file);
	return 0;
}

