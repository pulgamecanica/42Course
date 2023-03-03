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
# include <time.h>
# include <sys/stat.h>

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

enum	fileErrors {
	NoError,
	NotFounded,
	PermissionDenied
//	Regular_File = 4,          // '-'
//	Directory = 100,            // 'd'
//	Character_Device_File = 99, // 'c'
//	Block_Device_File = 98,     // 'b'
//	Local_Socket_File = 115,    // 's'
//	Named_Pipe = 112,           // 'p'
//	Symbolic_Link = 108         // 'l'
};

typedef struct s_file {
	enum fileErrors	f_errors;
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

void			ft_exit(int status, char * msg, bool msg_allocated);
void			free_file(void * ptr);
void			print_flags(ls_flags * flags);
void			ft_print_files(t_list * head, int depth, bool various);
void			setup_file(void * ptr, void * ptr2);
t_file *		init_file(char * str, char * path);
int				assign_flags(ls_flags * flags, char * str);
/* comparators */
int cmp_directory_last(void * ptr1, void * ptr2);
int cmp_ascii_order(void * ptr1, void * ptr2);
#endif
