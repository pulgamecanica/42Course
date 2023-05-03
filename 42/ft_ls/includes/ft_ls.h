#ifndef __FT_LS__
# define __FT_LS__

# ifndef DEBUG
#  define DEBUG 0
#endif

# ifndef LINUX
#  define LINUX 0
#endif

# define BLACK  "\033[0;30m"
# define RED  "\033[0;31m"
# define GREEN  "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define WHITE  "\033[0;37m"
# define ENDC "\033[0m"

# include "libft.h"
# include "ft_printf.h"
# include <stdint.h>
# include <stdbool.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
//# include <time.h>
//# include <dirent.h>
//# include <sys/types.h>
//# include <pwd.h>
//# include <grp.h>

# define PROGRAM_NAME "ft_ls"

# define ft_max(x,y) ((x) > (y) ? (x) : (y))

# define TIME_FORMAT "%b %e %Y"

# define MIN_COL_WIDTH 3

# define BOOL_TO_S(b) (b ? "true" : "false")

enum fileType {
  RegularFile = 45,       // '-'
  Directory = 100,        // 'd'
  CharDeviceFile = 99,    // 'c'
  BlockDeviceFile = 98,   // 'b'
  LocalSocketFile = 115,  // 's'
  NamedPipe = 112,        // 'p'
  SymbolicLink = 108,     // 'l'
  Unkown = -1
};


typedef struct s_file {
  /* The file name */
  char  * name;
  /* The path to the file */
  char  * path;
  /* When the file is sym link, the name of the file which is pointed else NULL */
  char  * link_name;
  /* The type of the file */
  enum  fileType fileType;
  /* File information which contains the group, password, permissions time; etc */
  struct stat stat;
  /* The lt_mode of the file linked */
  mode_t  link_mode;
  /* children of the directory */
  t_list  * children;
} t_file;


/* For each entry output, set the proper column width */
typedef struct s_padding {
  int inode_width;
  int block_size_width;
  int scontext_width;
  int owner_width;
  int group_width;
  int author_width;
  int major_device_width;
  int minor_device_width;
  int file_width;
} t_padding;

/* Output format printing */
enum format {
  LongFormat,     /* [-l] one entry per line long format */
  MultipleColumn, /* [default -C] multiple entry per column, order vertical */
  Horizontal,     /* [-x] multiple entry per column, order horizontal */
  OnePerLine,     /* [-1] one entry per line force */
  ComaSeparated   /* [-m] comma separated instead of columns, no padding*/
};

/* Sorting types */
enum sorting {
  SortNone, /* [-U] no sorting */
  SortName, /* [default] by file name */
  SortSize, /* [-S] by file size*/
  SortExt,  /* [-X] by file extension */
  SortTime  /* [-t] by time */
};

/* Indicator at the end of the file name */
enum file_indicator_type {
  FileIndicatorNone,      /* [default] */
  FileIndicatorSlash,     /* [-p] '/' */
  FileIndicatorClassify   /* [-F] by file class */
};

typedef struct s_conf {
  bool print_scontext;    /* Print scontext */
  enum sorting sorting;   /* Which sorting function to use */
  enum format format;     /* Which format to use */
  enum file_indicator_type fit; /* If we need to indicate file type at the end of name */
  t_padding current_padding;  /* Padding info for multiple columns or horizontal format */
  bool sort_rev;            /* [-r] reverse */
  bool print_owner;         /* [-g turn OFF] default true*/
  bool print_author;        /* Printing author? */
  bool print_group;         /* [-G] [-o turn OFF]*/
  bool print_block_size;    /* [-s] */
  bool print_inode;         /* [-i] */
  bool print_with_color;
  bool recursive;           /* [-R] */
  bool no_explore;           /* [-d] */
  bool dir_before_file;     /* [-d] */
  bool no_ignore;           /* [-a] default False*/
  bool print_dir;           /* print the directory before listing */
  bool need_stat;           /* indicates if it's needed to call stat */
  bool delimit;             /* delimit the options list */
  int line_len;             /* get the line lenght of the calling program */
  int max_cols;             /* maximum number of columns */
  uintmax_t block_size;
} t_conf;


// Conf
int init_conf(t_conf * conf);
void print_conf(t_conf * conf);

// Flags
bool add_flags(t_conf * conf, char * str);

#endif
