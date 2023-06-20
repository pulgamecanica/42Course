#include "ft_ls.h"

static void set_padding_file_width(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	file = (t_file *)ptr1;
	conf = (t_conf *)ptr2;
	if (!file || !conf)
		return ;
	conf->padding.file_width = ft_max(conf->padding.file_width, (int)ft_strlen(file->name));
}

static void set_padding_file_links(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	file = (t_file *)ptr1;
	conf = (t_conf *)ptr2;
	if (!file || !conf)
		return ;
	conf->padding.file_links_width = ft_max(conf->padding.file_links_width, ft_numlen_base(file->stat.st_nlink, 10));
}

static void set_padding_owner_author_width(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	file = (t_file *)ptr1;
	conf = (t_conf *)ptr2;
	if (!file || !conf)
		return ;
	conf->padding.owner_width = ft_max(conf->padding.owner_width, (int)ft_strlen(getpwuid(file->stat.st_uid)->pw_name));
	conf->padding.author_width = conf->padding.owner_width;
}


static void set_padding_group_width(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	file = (t_file *)ptr1;
	conf = (t_conf *)ptr2;
	if (!file || !conf)
		return ;
	conf->padding.group_width = ft_max(conf->padding.group_width, (int)ft_strlen(getgrgid(file->stat.st_gid)->gr_name));
}

static void set_padding_inode_width(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	file = (t_file *)ptr1;
	conf = (t_conf *)ptr2;
	if (!file || !conf)
		return ;
	conf->padding.inode_width = ft_max(conf->padding.inode_width, ft_numlen_base(file->stat.st_ino, 10));
}

static void set_padding_block_size(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	file = (t_file *)ptr1;
	conf = (t_conf *)ptr2;
	if (!file || !conf)
		return ;
	conf->padding.block_size_width = ft_max(conf->padding.block_size_width, ft_numlen_base(file->stat.st_blocks / 2, 10));
}

static void set_padding_file_size(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	file = (t_file *)ptr1;
	conf = (t_conf *)ptr2;
	if (!file || !conf)
		return ;
	conf->padding.file_size_width = ft_max(conf->padding.file_size_width, ft_numlen_base(file->stat.st_size, 10));
}

void set_padding(t_list * list, t_conf * conf) {
	if (conf->format != LongFormat)
		return ;
	ft_bzero(&conf->padding, sizeof(t_padding));
	if (conf->print_inode)
		ft_lstiter_param(list, set_padding_inode_width, conf);
	if (conf->print_block_size)
		ft_lstiter_param(list, set_padding_block_size, conf);
  if (conf->print_group)
		ft_lstiter_param(list, set_padding_group_width, conf);
	if (conf->print_owner || conf->print_author)
		ft_lstiter_param(list, set_padding_owner_author_width, conf);
//??
	conf->padding.major_device_width = 0;
//??
  conf->padding.minor_device_width = 0;
//	??
//  if (conf->print_scontext)
//	  conf->padding.scontext_width = 0;
	ft_lstiter_param(list, set_padding_file_links, conf);
  ft_lstiter_param(list, set_padding_file_size, conf);
  ft_lstiter_param(list, set_padding_file_width, conf);
  if (DEBUG)
	  ft_printf("PADDING ==> Inode[%d] Block[%d] Contxt[%d] Owner[%d] Grp[%d] Auth[%d] MJD[%d] mnd[%d] name[%d] size[%d]\n",
	  	conf->padding.inode_width,
			conf->padding.block_size_width,
			conf->padding.scontext_width,
			conf->padding.owner_width,
			conf->padding.group_width,
			conf->padding.author_width,
			conf->padding.major_device_width,
			conf->padding.minor_device_width,
			conf->padding.file_width,
			conf->padding.file_size_width);
}

char * format_padding(char conversion, int padding, bool space, bool minus) {
	char * result;
	char * padding_str;
	char tmp[3];

	tmp[0] = 0;
	tmp[1] = 0;
	tmp[2] = 0;
	padding_str = ft_itoa(padding);
	result = ft_strjoin(minus ? ft_strdup("%-") : ft_strdup("%"), padding_str);
	if (space) {
		tmp[0] = ' ';
		tmp[1] = conversion;
	} else {
		tmp[0] = conversion;
	}
	result = ft_strjoin(result, tmp);
	free(padding_str);
	return (result);
}