#include "file.h"
#include "conf.h"
#include "libft.h"
#include "ft_printf.h"


void	setup_padding(void * ptr1, void * ptr2) {
	t_file * f;
	t_conf * c;

	f = (t_file *)ptr1;
	c = (t_conf *)ptr2;
	if (!f || !c)
		return ;
	if (c->inode)
		c->padding.inode = ft_max(c->padding.inode, ft_numlen_base(f->stat.st_ino, 10));
	if (c->size)
		c->padding.block_size = ft_max(c->padding.block_size, ft_numlen_base(f->stat.st_blocks / 2, 10));
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