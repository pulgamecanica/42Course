#include "conf.h"
#include "libft.h"

static t_long_options new_default_long_options() {
	t_long_options l_opt;

	l_opt.owner = true;
	l_opt.author = false;
	l_opt.group = true;
	l_opt.block_size = false;
	l_opt.bytes_per_block = 1024;
	l_opt.numeric_uid_gid = false;
	return (l_opt);
}

t_conf * new_default_conf_ls() {
	t_conf	* c;
	c = ft_calloc(sizeof(t_conf), 1);
	if (!c)
		return (NULL);

	c->format = one_per_line;
	c->sorting = by_name;

	/* Will hold the long options when long format is on*/
	c->l_opts = new_default_long_options();
	
	/* Don't show inode by default */
	c->inode = false;
	/* Don't show size by default */
	c->size = false;
	/* No color by default */
	c->color = false;
	/* Sort normal by default */
	c->sort_rev = false;
	/* Ignore by default */
	c->no_ignore = false;
	/* Ignore almost all by default */
	c->almost_no_ignore = false;
	/* Explore directories by default */
	c->no_explore = false;
	/* Not recursive by default */
	c->recursive = false;
	/* False by default unless '--' founded */
	c->delimit = false;
	/* False by default, don't print indicators */
	c->print_indicator = false;
	/* Stat only needed when long options is on */
	c->need_stat = false;

	/* Only print directory if there is more than one entry */
	c->print_dir = false;
	/* If any parameters are printed before file listing, there must be a separation */
	c->initial_separation = false;
	return (c);
}