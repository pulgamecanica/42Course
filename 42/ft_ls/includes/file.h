#ifndef __FT_FILE__
#define __FT_FILE__

# include <stdbool.h>
# include <sys/stat.h>
# include <stdlib.h>

# define REGULAR_FILE 45      // -
# define DIRECTORY 100        // d
# define SYMBOLIC_LINK 108    // l
# define CHAR_DEVISE_FILE 99  // c
# define BLOCK_DEVISE_FILE 98 // b
# define SOCKET_FILE 115      // s
# define NAMED_PIPE 112       // p
# define MISSING_FILE 63      // ?

enum file_indicator_type {
	regular_file = REGULAR_FILE,
	directory = DIRECTORY,
	symbolic_link = SYMBOLIC_LINK,
	char_device_file = CHAR_DEVISE_FILE,
	block_device_fille= BLOCK_DEVISE_FILE,
	socket_file = SOCKET_FILE,
	named_pipe = NAMED_PIPE,
	missing_file = MISSING_FILE
};


typedef struct s_file {
	char	* name;
	char	* path;
	enum file_indicator_type fit;
	struct stat stat;
	/* Fields bellow only used when file is a symbolic link */
	/* The lt_mode and link_name of the file linked */
	char	* link_name;
	struct stat lstat;
	enum file_indicator_type lit;
	/* String with the scontext*/
	char * scontext;
}	t_file;

t_file * new_file(char * name, char * path);
void	free_file(void * ptr);
void	setup_scontext(void * ptr);
#endif
