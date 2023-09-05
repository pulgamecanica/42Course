#include "file.h"
#include "ft_printf.h"
#include "libft.h"
#include "color.h"
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

/**
 * About the block size and file size
 * 
 * Block size is the amout of blocks which the OS allocated
 * to store the file, this allocated space not always corresponds
 * exactly to the file size, some times the allocated space is way higher
 * 
 * We can get the number of blocks allocated using the stat family functions
 * It returns the number of blocks 512B each, so for 1 block it would be 512B
 * You can prove this by doing a quick ls -s and multiply the number of blocks
 * which will always correspond to the file size or more.
 * 
 * The only thing is... ls will return the number of blocks but in this case each 
 * block is 1024B, because that's the default and on the manual it is said
 * that you should concider 1024B blocks. So it will have LESS blocks compared to
 * the stat value. That is why I divide the st_blocks / 2 so I can get the coorespondent value.
 * 
 * 512B		->	1block (in stat function)
 * 1024B	->	1block (ls -s)
 * 
 * Dividing the number of 512B / 2 will get the number of 1024B blocks.
 **/

// ls will clean the color on the first color printing
// I wont bother escaping the sequence because never the less the color will be changed to something else
// I dont need to escape the sequence, I don't know why ls does it, I won't replicate it
/**
 * >$ ls --color=always -1a | cat -e
 * You can see the result for yourself, the output when color is present will always have an escape when it starts 
**/

// [DONE] Normal Files No Color
// [DONE] Executable files Green Bold
// [DONE] File with set-user-ID bit gray with red background
// [DONE] File with set-group-ID bit black with yellow background  
// [DONE] Directories Blue Bold
// [DONE] Directories with other writting permission, blue with green background
// [DONE] Char device files yellow with black background
// [DONE] Symbolic Link files Cyan Bold
// [DONE] Named pipe yellow with black background
// [DONE] Socket files Purple Bold
// Unexplainable behaviour:
// When you have a file named: # for some reason is colored gray...
static char * get_file_color(enum file_indicator_type fit, mode_t mode) {
	switch (fit) {
		case DIRECTORY:
			if (mode & S_IWOTH)
				return (NULL);
			else
				return (BLUEBOLD);
		case REGULAR_FILE:
			if (mode & S_ISUID)
				return (GRAY_RED_BG);
			if (mode & S_ISGID)
				return (BLACK_YELLOW_BG);
			if (mode & S_IXGRP)
				return (GREENBOLD);
			else
				return (NULL);
		case SYMBOLIC_LINK:
			return (CYANBOLD);
		case SOCKET_FILE:
			return (PURPLEBOLD);
		case NAMED_PIPE:
			return (YELLOW_DARK_BG);
		default: return (NULL);
	}
}

// Print functions shared by all formats :)
static void print_file_name(char * name, enum file_indicator_type fit, mode_t mode, t_conf * c) {
	char * str;

	str = c->color ? get_file_color(fit, mode) : NULL;
	if (str)
		ft_printf("%s%s%s%s", str, name, ENDC, c->print_indicator && fit == DIRECTORY ?  "/" : "");
	else
		ft_printf("%s%s", name, c->print_indicator && fit == DIRECTORY ?  "/" : "");
}

static void print_inode(t_file * f, t_conf * c) {
	char * str;

	if (!(str = ft_calloc(sizeof(char), 10))) // I doubt we can reach paddings for numbers higher than 5 digits
		return ;
	sprintf(str, "%%%dd ", c->padding.inode);
	ft_printf(str, f->stat.st_ino);
	free(str);
}

static void print_block_size(t_file * f, t_conf * c) {
	char * str;

	if (!(str = ft_calloc(sizeof(char), 10))) // I doubt we can reach paddings for numbers higher than 5 digits
		return ;
	sprintf(str, "%%%dd ", c->padding.block_size);
	ft_printf(str, f->stat.st_blocks / 2);
	free(str);
}

/**
 * One per line analisis
 *  [X] ls -g -> activates long format
 *  [O] ls -i -> print inode number before file name 
 *  [X] ls -k -> printing not affected
 *  [X] ls -m -> activates comma separated format
 *  [X] ls -n -> activates long format
 *  [X] ls -o -> activates long format
 *  [O] ls -p -> print a slash after any directory
 *  ls -s -> print the file size before file name and after inode
 *					 also activates the total being printed before the file listing
 *  ls -Z -> print the security contect before the file name and after the size and after the inode
 * 
 * Most verbose version: ls -1Zisp
 * 4812565  0 ? bin/
 * |        | | |  L indicator
 * |        | | |
 * |        | | L file name
 * |        | |
 * |        | L security context (Not implementd for the moment)
 * |        | 
 * |        | 
 * |        L size
 * |  
 * L inode  
 * 
*/
static void one_per_line_format_f(void * ptr1, void * ptr2) {
	t_file * f;
	t_conf * c;

	f = (t_file *)ptr1;
	c = (t_conf *)ptr2;
	if (!f || !c)
		return ;
	if (c->inode)
		print_inode(f, c);
	if (c->block_size)
		print_block_size(f, c);
	// if (context)
		// ft_printf("%s ", 42);
	print_file_name(f->name, f->fit, f->stat.st_mode, c);
	write(1, "\n", 1);
}

static char x_usr_mode_bits(mode_t mode) {
	if (mode & S_IXUSR)
		return ('x');
	if (mode & S_ISUID)
		return ('s');
	return ('-');
}

static char x_grp_mode_bits(mode_t mode) {
	if (mode & S_IXGRP)
		return ('x');
	if (mode & S_ISGID)
		return ('s');
	return ('-');
}

/**
 * Long Format analisis 
 * drwxr-xr-x 1 pulgamecanica pulgamecanica   164 Feb 19 18:42 .
 * ││       │ │ │           │ │           │   │ │ │          │ │
 * ││       │ │ │           │ │           │   │ │ │          │ └ file name 
 * ││       │ │ │           │ │           │   │ │ │          │
 * ││       │ │ │           │ │           │   │ │ └──────────└ timestamp
 * ││       │ │ │           │ │           │   │ │
 * ││       │ │ │           │ │           │   └─└ size
 * ││       │ │ │           │ │           │
 * ││       │ │ │           │ └───────────└ group name
 * ││       │ │ │           │
 * ││       │ │ └───────────└  owner name
 * ││       │ │
 * ││       │ └ number of hard links
 * ││       │
 * │└───────└ file mode bits
 * │
 * └─── file type
 *
 * EXAMPLE MOST VERBOSE
 * 
 * >$ ls -l --inode --size --author --context -p 
 * or
 * >$ ls -l -i -s --author -Z -p 
 *
 * Inode   | block size | permissions | #links | owner        | group | size (MB) | last modified  | name  |  link_name 
 * 3918521   48          -rwxr-xr-x     1        pulgamecanica  root    47320       May  5 12:31     ft_ls -> some_link 
 * 
 * [DONE] inode
 * [DONE] block_size
 * [DONE] file_tyle
 * [DONE] permissions
 * [DONE] # links
 * [DONE] owner
 * [DONE] group
 * [DONE] autor
 * [] scontext (security context)
 * [DONE] size (MB 1024)
 * [DONE] last modified
 * [DONE] name
 * [DONE] indicator
 * [DONE] -> link name (only for links)
 * [DONE] link indicator
 * 
 **/

// I will clean this function later ofcs, I will make the str_format volatile

static void long_format_f(void * ptr1, void *ptr2) {
	t_file * f;
	t_conf * c;
	char * str_format;

	f = (t_file *)ptr1;
	c = (t_conf *)ptr2;
	if (!f || !c)
		return ;
	if (c->inode)
		print_inode(f, c);
	if (c->block_size)
		print_block_size(f, c);
	// File Type and Mode bits, USR, GRP, OTH
	ft_printf("%c%c%c%c%c%c%c%c%c%c ", 
		f->fit,
		f->stat.st_mode & S_IRUSR ? 'r' : '-',
		f->stat.st_mode & S_IWUSR ? 'w' : '-',
		x_usr_mode_bits(f->stat.st_mode),
		f->stat.st_mode & S_IRGRP ? 'r' : '-',
		f->stat.st_mode & S_IWGRP ? 'w' : '-',
		x_grp_mode_bits(f->stat.st_mode),
		f->stat.st_mode & S_IROTH ? 'r' : '-',
		f->stat.st_mode & S_IWOTH ? 'w' : '-',
		f->stat.st_mode & S_IXOTH ? 'x' : '-'
	);
	// Number of links
	str_format = ft_calloc(sizeof(char), 10);
	sprintf(str_format, "%%%dd ", c->padding.links);
	ft_printf(str_format, f->stat.st_nlink);
	free(str_format);
	// Owner
	if (c->l_opts.owner) {
		str_format = ft_calloc(sizeof(char), 10);
		if (c->l_opts.numeric_uid_gid) {
			sprintf(str_format, "%%%dd ", c->padding.owner);
			ft_printf(str_format, f->stat.st_uid);
		} else {
			sprintf(str_format, "%%-%ds ", c->padding.owner);
			ft_printf("%s ", getpwuid(f->stat.st_uid)->pw_name);
		}
		free(str_format);
	}
	// Group
	if (c->l_opts.group) {
		str_format = ft_calloc(sizeof(char), 10);
		if (c->l_opts.numeric_uid_gid) {
			sprintf(str_format, "%%%dd ", c->padding.group);
			ft_printf(str_format, f->stat.st_gid);
		} else {
			sprintf(str_format, "%%-%ds ", c->padding.group);
			ft_printf(str_format, getgrgid(f->stat.st_gid)->gr_name);
		}
		free(str_format);
	}
	// Author
	if (c->l_opts.author) {
		str_format = ft_calloc(sizeof(char), 10);
		if (c->l_opts.numeric_uid_gid) {
			sprintf(str_format, "%%%dd ", c->padding.author);
			ft_printf(str_format, f->stat.st_uid);
		} else {
			sprintf(str_format, "%%-%ds ", c->padding.author);
			ft_printf(str_format, getpwuid(f->stat.st_uid)->pw_name);
		}
		free(str_format);
	}
	// scontext
	// size MB (1024 chunks each block) block size will always be equal or higher than size / 1024
	str_format = ft_calloc(sizeof(char), 10);
	sprintf(str_format, "%%%dd ", c->padding.size);
	ft_printf(str_format, f->stat.st_size);
	free(str_format);
	// last modified
	str_format = ft_substr(ctime(&f->stat.st_mtime), 4, 12);
	ft_printf("%s ", str_format);
	free(str_format);
	// file name
	print_file_name(f->name, f->fit, f->stat.st_mode, c);
	// link name
	if (f->fit == SYMBOLIC_LINK && f->link_name) {
		ft_putstr_fd(" -> ", 1);
		print_file_name(f->link_name, f->lit, f->lstat.st_mode, c);
	}
	write(1, "\n",1);
}

void * format_func(enum format f) {
	switch (f) {
		case one_per_line: return one_per_line_format_f;
		case multiple_in_column: return one_per_line_format_f;
		case multiple_in_row: return one_per_line_format_f;
		case long_format: return long_format_f;
		case coma_separated: return one_per_line_format_f;
		default: return one_per_line_format_f;
	}
}
