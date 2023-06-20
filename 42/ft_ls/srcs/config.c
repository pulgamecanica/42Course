#include "ft_ls.h"
#include "libft.h"

static const char *format_names[] = {
	"Long", "Multiple Column", "Horizontal", "One Per Line", "Coma Separated", NULL
};

static const char *sorting_names[] = {
	"None", "By Name", "By Size", "By Extension", "By Time", NULL
};

static const char *indicator_names[] = {
	"None", "Slash", "Classify", NULL
};

void print_conf(t_conf * conf) {
	if (!conf)
		return ;
	ft_printf(""
		"|-------------------------------------------------------------------------------------------------------------------|\n"
		"| Configs | Scontext | Sorting    | Format     | Indicator | Rev Sort | Owner | Author | Group | Block sz on |      |\n"
		"| ------- | -------- | ---------- | ---------- | --------- | -------- | ----- | ------ | ----- | ----------- | ---- |\n"
		"|\033[0;34m Values: | %-8 .8s | %-10 .10s | %-10 .10s | %-9 .9s | %-8 8s | %-5 5s | %-6 6s | %-5 5s | %-11 11s |      \033[0m|\n"
		"|-------------------------------------------------------------------------------------------------------------------|\n"
		"| Configs | Inode | Color | Recursive | No Explore | Dir Before | line len | Max  Cols | Need Stats | Block size    |\n"
		"| ------- | ----- | ----- | --------- | ---------- | ---------- | -------- | --------- | ---------- | ------------- |\n"
		"|\033[0;34m Values: | %-5 .5s | %-5 .5s | %-9 .9s | %-10 .10s | %-10 .10s | %08d | %09d | %-10 .10s | %013d \033[0m|\n"
		"|-------------------------------------------------------------------------------------------------------------------|\n"
		"| Configs | No ignore | Print Dirs | Delimited |\033[0;33m\033[1m  |---     T         |      -----      -+-        /  '   -'''|      \033[0m|\n"
		"| ------- | --------- | ---------- | --------- |\033[0;33m\033[1m -|---   --|--  ===  |      |___.     -+o+-      /___'    ___|      \033[0m|\n"
		"|\033[0;34m Values: | %-9 .9s | %-10 .10s | %-9 .9s |\033[0;33m\033[1m  L_      _|_        L___   ____|      -+-          _'_  |____      \033[0m|\n"
		"|-------------------------------------------------------------------------------------------------------------------|\n",
		BOOL_TO_S(conf->print_scontext), sorting_names[conf->sorting], format_names[conf->format],
		indicator_names[conf->fit], BOOL_TO_S(conf->sort_rev), BOOL_TO_S(conf->print_owner),
		BOOL_TO_S(conf->print_author), BOOL_TO_S(conf->print_group), BOOL_TO_S(conf->print_block_size),
		BOOL_TO_S(conf->print_inode), BOOL_TO_S(conf->print_with_color), BOOL_TO_S(conf->recursive),
		BOOL_TO_S(conf->no_explore), BOOL_TO_S(conf->dir_before_file), conf->line_len,
		conf->max_cols, BOOL_TO_S(conf->need_stat), conf->block_size,
		BOOL_TO_S(conf->no_ignore), BOOL_TO_S(conf->print_dir), BOOL_TO_S(conf->delimit)
	);
}

t_conf *	init_conf(void) {
	struct winsize	sz;
	t_conf * conf;

	conf = (t_conf *)ft_calloc(sizeof(t_conf), 1);
	if (!conf)
		return (NULL);
	conf->sorting = SortName;
	conf->format = LongFormat;//MultipleColumn;
	conf->fit = FileIndicatorNone;
	conf->params_on = true;
	conf->print_owner = true;
	conf->print_group = true;
	conf->print_with_color = true;
	conf->block_size = 1;
	conf->max_cols = 4;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &sz);
	conf->line_len = sz.ws_col;
	if (!conf->line_len)
		conf->print_with_color = false;
	return (conf);
}
