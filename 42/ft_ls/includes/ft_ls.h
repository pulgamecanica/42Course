#ifndef __FT_LS__
# define __FT_LS__

# ifndef DEBUG
#  define DEBUG 0
# endif

# include "libft.h"
# include "ft_printf.h"
# include <stdbool.h>
# include <stdlib.h>
# include <sys/types.h>
# include <dirent.h>

typedef struct ls_flags {
	bool flagl; // -l
	bool flagR; // -R --recursive
	bool flaga; // -a --all
	bool flagr; // -r --reverse
	bool flagt; // -t
	bool delimit; // -- delimit the options list
}	ls_flags;

typedef struct ls_config {
	ls_flags	flags;
	t_list *	files;
}	ls_config;

enum	filetype {
	NotFounded = 0,
	Regular_File = 45,          // '-'
	Directory = 100,            // 'd'
	Character_Device_File = 99, // 'c'
	Block_Device_File = 98,     // 'b'
	Local_Socket_File = 115,    // 's'
	Named_Pipe = 112,           // 'p'
	Symbolic_Link = 108         // 'l'
};

typedef struct s_file {
	enum filetype	f_type;
	ino_t		d_ino;
	size_t		f_size;
	size_t		f_create_date;
	size_t		f_last_modify;
	char *		f_name;
	int				f_mode;
	int				total;
	t_list *	children;
}	t_file;

# define ft_max(x,y) ((x) > (y) ? (x) : (y))

void			ft_exit(int status, char * msg, bool msg_allocated);
void			free_file(void * ptr);
void			print_flags(ls_flags * flags);
void			ft_print_files(t_list * head);
void			setup_files(ls_config * config);
t_file *	init_file(char * str);
int				assign_flags(ls_flags * flags, char * str);

#endif
