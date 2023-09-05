#include "file.h"
#include "conf.h"
#include "libft.h"
#include <pwd.h>
#include <grp.h>

void	setup_padding(void * ptr1, void * ptr2) {
	t_file * f;
	t_conf * c;

	f = (t_file *)ptr1;
	c = (t_conf *)ptr2;
	if (!f || !c)
		return ;
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
	int * n;

	f = (t_file *)ptr1;
	n = (int *)ptr2;
	if (!n || !f)
		return ;
	*n += f->stat.st_blocks / 2;
}
