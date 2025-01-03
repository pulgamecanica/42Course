#include "file.h"
#include "conf.h"
#include "libft.h"
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdint.h>


int ft_ceil(float num) {
    int inum;

    inum = (int)num;
    if (num == (float)inum)
        return inum;
    return (inum + 1);
}

/**
 * How to calculate 6 months from now? How much is 6 months?
 * One year has:
 * Days: 365.2422
 * Hours: Days * 24
 * Minutes: Hours * 60
 * Seconds: Minutes * 60
 * (==) Seconds: 365.2422 * 24 * 60 * 60 = 31556926.08
 * Therefore 6 months has half of that:
 * 31556926.08 / 2 = 15778463.04 ~> 15778463 
 **/
void	setup_six_months_from_now(t_conf * conf) {
	// time_t should be integer, and point the number of seconds since EPOCH
	time_t current_time;
	time(&current_time);
	conf->six_months_from_now = current_time - 15778463;
}


void	setup_padding(void * ptr1, void * ptr2) {
	t_file * f;
	t_conf * c;

	f = (t_file *)ptr1;
	c = (t_conf *)ptr2;
	if (!f || !c)
		return ;
	if (c->scontext || c->format == long_format)
		c->padding.scontext = ft_max(c->padding.scontext, f->scontext ? (int)ft_strlen(f->scontext) : 0);
	if (c->inode)
		c->padding.inode = ft_max(c->padding.inode, ft_numlen_base(f->stat.st_ino, 10));
	if (c->block_size)
		c->padding.block_size = ft_max(c->padding.block_size, ft_numlen_base(f->stat.st_blocks / 2, 10));
	if (c->format != long_format)
		return ;
	c->padding.links = ft_max(c->padding.links, ft_numlen_base(f->stat.st_nlink, 10));
	if (c->l_opts.numeric_uid_gid) {
		c->padding.owner = ft_max(c->padding.owner, ft_numlen_base(f->stat.st_uid, 10));
		c->padding.group = ft_max(c->padding.group, ft_numlen_base(f->stat.st_gid, 10));
		c->padding.author = c->padding.owner;
	} else {
		c->padding.owner = ft_max(c->padding.owner, (int)ft_strlen(getpwuid(f->stat.st_uid)->pw_name));
		c->padding.group = ft_max(c->padding.group, (int)ft_strlen(getgrgid(f->stat.st_gid)->gr_name));
		c->padding.author = c->padding.owner;
	}
	c->padding.size = ft_max(c->padding.size, ft_numlen_base(f->stat.st_size, 10));
}

void	setup_total(void * ptr1, void * ptr2) {
	t_file * f;
	uintmax_t * n;

	f = (t_file *)ptr1;
	n = (uintmax_t *)ptr2;
	if (!n || !f)
		return ;
	*n += f->stat.st_blocks;
}
