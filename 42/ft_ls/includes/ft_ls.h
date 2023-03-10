#ifndef __FT_LS__
# define __FT_LS__

# ifndef DEBUG
#  define DEBUG 0
# endif

# include "libft.h"
# include "ft_printf.h"
# include <time.h>
# include <stdlib.h>
# include <dirent.h>
# include <stdbool.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/ioctl.h>

typedef struct ls_flags {
	bool flagl; // -l [show long]
	bool flagR; // -R --recursive [show recursive]
	bool flaga; // -a --all [show all]
	bool flagr; // -r --reverse [sort reverse]
	bool flagt; // -t [sort time]
	bool flagL; // -L [show when link, show the file not the link]
	bool flagD; // -D [show Dired] (only when -l)
	bool flagu; // -u [show time creation timestamp] (only when -l)
	bool flagf; // -f [show unsorted directory listing --allocated --sort=none, -1 & order matters]
	bool flagg; // -g [show long format no owner info]
	bool flagd; // -d [show directory name only, no listing]
	bool flag1; // -1 [show one file per line & order matters]
	bool flagp; // -p [show '/' when file is directory]
	bool flagm; // -m [show coma separator columnated, disable -l & order matters]
	bool flagx; // -x [show files in columns & order matters]
	bool flagi; // -i [show innode number]
	bool flagC; // -i [list by columns]
	bool delimit; // -- delimit the options list
}	ls_flags;

enum	format {
	long_format = 0,	/* -l */
	multiple_column,	/* default */
	one_per_line,		/* -1 */
	horizontal,			/* -x */
};

enum	sorting_strategy {
	name_sort = 0,
	extension_sort,
	size_sort,
	time_sort,
	none_sort,
	numtypes_sort
};

enum time_style {
	full_iso = 0,
	long_iso,
	locale,
	iso
};

typedef struct ls_config {
	ls_flags	flags;
	t_list *	files;
	struct winsize	sz;
}	ls_config;

enum	fileErrors {
	NoError,
	NotFounded,
	PermissionDenied
};

enum	fileType {
	Regular_File = 45,			// '-'
	Directory = 100,			// 'd'
	Character_Device_File = 99,	// 'c'
	Block_Device_File = 98,		// 'b'
	Local_Socket_File = 115,	// 's'
	Named_Pipe = 112,			// 'p'
	Symbolic_Link = 108,		// 'l'
	Unknown = -1
};

typedef struct s_file {
	struct s_file * f_link_file;
	enum fileErrors	f_errors;
	enum fileType	f_type;
	ino_t		d_ino;
	struct stat	f_stat;
	char *		f_name;
	char *		f_path;
	int			total;
	t_list *	children;
}	t_file;


//struct stat {
//   dev_t     st_dev;         /* ID of device containing file */
//   ino_t     st_ino;         /* Inode number */
//   mode_t    st_mode;        /* File type and mode */
//   nlink_t   st_nlink;       /* Number of hard links */
//   uid_t     st_uid;         /* User ID of owner */
//   gid_t     st_gid;         /* Group ID of owner */
//   dev_t     st_rdev;        /* Device ID (if special file) */
//   off_t     st_size;        /* Total size, in bytes */
//   blksize_t st_blksize;     /* Block size for filesystem I/O */
//   blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */
//
//   /* Since Linux 2.6, the kernel supports nanosecond
//      precision for the following timestamp fields.
//      For the details before Linux 2.6, see NOTES. */
//
//   struct timespec st_atim;  /* Time of last access */
//   struct timespec st_mtim;  /* Time of last modification */
//   struct timespec st_ctim;  /* Time of last status change */
//
//#define st_atime st_atim.tv_sec      /* Backward compatibility */
//#define st_mtime st_mtim.tv_sec
//#define st_ctime st_ctim.tv_sec
//};

# define ft_max(x,y) ((x) > (y) ? (x) : (y))

/* utils */
void			ft_exit(int status, char * msg, bool msg_allocated);
/* files */
void		free_file(void * ptr);
void		ft_print_files(t_list * head, int depth, bool various);
void		ft_print_directory_files(void * ptr1, void * ptr2);
void		ft_print_regular_files(ls_config * conf);
void		setup_file(void * ptr, void * ptr2);
t_file *	init_file(char * str, char * path);
/* flags */
int		assign_flags(ls_flags * flags, char * str);
void	print_flags(ls_flags * flags);
/* comparators */
int cmp_not_exist_first(void * ptr1, void * ptr2);
int cmp_rev_not_exist_first(void * ptr1, void * ptr2);
int	cmp_ascii_order(void * ptr1, void * ptr2);
int cmp_rev_ascii_order(void * ptr1, void * ptr2);
/* formats */
char	* horizontal_format(t_file * parent, ls_config * config);
char	* multiple_column_format(t_file * parent, ls_config * config);
char	* long_format_format(t_file * parent, ls_config * config);
char	* one_per_line_format(t_file * parent, ls_config * config);


#endif
