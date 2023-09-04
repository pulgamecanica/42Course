#include "file.h"
#include "ft_printf.h"
#include "libft.h"
#include "color.h"

/*
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

// ls will clean the color on the first color printing
// I wont bother escaping the sequence because never the less the color will be changed to something else
// I dont need to escape the sequence, I don't know why ls does it, I won't replicate it
/**
 * >$ ls --color=always -1a | cat -e
 * You can see the result for yourself, the output when color is present will always have an escape when it starts 
**/
static char * get_file_color(t_file * file) {
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
	// Not clear behaviour:
	// When you have a file named: # for some reason is colored gray...
	switch (file->fit) {
		case DIRECTORY:
			if (file->stat.st_mode & S_IWOTH)
				return (NULL);
			else
				return (BLUEBOLD);
		case REGULAR_FILE:
			if (file->stat.st_mode & S_ISUID)
				return (GRAY_RED_BG);
			if (file->stat.st_mode & S_ISGID)
				return (BLACK_YELLOW_BG);
			if (file->stat.st_mode & S_IXGRP)
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

static void one_per_line_format_f(void * ptr1, void * ptr2) {
	t_file * f;
	t_conf * c;
	char * str_format;

	f = (t_file *)ptr1;
	c = (t_conf *)ptr2;
	if (!f || !c)
		return ;
	if (c->inode) {
		str_format = ft_calloc(sizeof(char), 10);  // I doubt we can reach paddings for numbers higher than 5 digits
		sprintf(str_format, "%%%dd ", c->padding.inode);
		ft_printf(str_format, f->stat.st_ino);
		free(str_format);
	}
	if (c->size) {
		str_format = ft_calloc(sizeof(char), 10);
		sprintf(str_format, "%%%dd ", c->padding.block_size);
		ft_printf(str_format, f->stat.st_blocks / 2);
		free(str_format);
	}
	// if (context)
		// ft_printf("%s ", 42);
	str_format = c->color ? get_file_color(f) : NULL;
	if (str_format)
		ft_printf("%s%s%s%s\n", str_format, f->name, ENDC, c->print_indicator && f->fit == DIRECTORY ?  "/" : "");
	else
		ft_printf("%s%s\n", f->name, c->print_indicator && f->fit == DIRECTORY ?  "/" : "");
}

static void long_format_f(void * ptr1, void *ptr2) {
	t_file * f;
	(void)ptr2;
	f = (t_file *)ptr1;
	if (!f)
		return ;
	ft_printf("long format -> %s\n", f->name);
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