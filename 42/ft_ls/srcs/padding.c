#include "ft_ls.h"

/**
 * concretize
 * This function will concretize the abstraction from
 * the linked list which reproduces a void *
 **/
static bool concretize(t_conf ** conf, t_file ** file, void * ptr1, void * ptr2) {
	*file = (t_file *)ptr1;
	*conf = (t_conf *)ptr2;
	if (*file && *conf)
		return (true);
	return (false);
}

/**
 * set_padding_*
 * This functions will set the configuration
 * padding for the given file.
 * 1 - set_padding_file_width
 * 2 - set_padding_file_links
 * 3 - set_padding_owner_author_width
 * 4 - set_padding_group_width
 * 5 - set_padding_inode_width
 * 6 - set_padding_block_size
 * 7 - set_padding_file_size
 **/

/* 1 */
static void set_padding_file_width(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	if (concretize(&conf, &file, ptr1, ptr2))
		conf->padding.file_width = ft_max(conf->padding.file_width, (int)ft_strlen(file->name));
}
/* 2 */
static void set_padding_file_links(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	if (concretize(&conf, &file, ptr1, ptr2))
		conf->padding.file_links_width = ft_max(conf->padding.file_links_width, ft_numlen_base(file->stat.st_nlink, 10));
}
/* 3 */
static void set_padding_owner_author_width(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	if (!concretize(&conf, &file, ptr1, ptr2))
		return ;
	conf->padding.owner_width = ft_max(conf->padding.owner_width, (int)ft_strlen(getpwuid(file->stat.st_uid)->pw_name));
	conf->padding.author_width = conf->padding.owner_width;
}
/* 4 */
static void set_padding_group_width(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	if (concretize(&conf, &file, ptr1, ptr2))
		conf->padding.group_width = ft_max(conf->padding.group_width, (int)ft_strlen(getgrgid(file->stat.st_gid)->gr_name));
}
/* 5 */
static void set_padding_inode_width(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	if (concretize(&conf, &file, ptr1, ptr2))
		conf->padding.inode_width = ft_max(conf->padding.inode_width, ft_numlen_base(file->stat.st_ino, 10));
}
/* 6 */
static void set_padding_block_size(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	if (concretize(&conf, &file, ptr1, ptr2))
		conf->padding.block_size_width = ft_max(conf->padding.block_size_width, ft_numlen_base(file->stat.st_blocks / 2, 10));
}
/* 7 */
static void set_padding_file_size(void * ptr1, void * ptr2) {
	t_conf * conf;
	t_file * file;

	if (concretize(&conf, &file, ptr1, ptr2))
		conf->padding.file_size_width = ft_max(conf->padding.file_size_width, ft_numlen_base(file->stat.st_size, 10));
}

/**
 * set_padding
 * 
 * */
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


/**
 * format_padding
 * Will generate a desired format for ft_printf
 * The format should always incude the padding
 * Ex: "%4d" // Padding of 4 characters
 * There are two optional booleans: space & minus
 * Ex: "%- d" or "%-s" or "% d"
 * Space: Will fill the padding empty space with spaces insteed of zero
 * Minus: Will right justify the content
 * 
 * INFO: I could have used sprintf, but that would be cheating IMO
 * */
char * format_padding(char conversion, int padding, bool space, bool minus) {
	char * result;
	char * padding_str;
	char tmp[3];

	ft_bzero(tmp, 3);
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