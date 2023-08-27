#ifndef __FT_LS__
# define __FT_LS__

# ifndef DEBUG
#  define DEBUG 0
#endif

# ifndef LINUX
#  define LINUX 0
#endif

# include "libft.h"
# include "ft_printf.h"
# include <stdint.h>
# include <stdbool.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <time.h>
# include <grp.h>
# include <pwd.h>
# include <dirent.h>

# define PROGRAM_NAME "ft_ls"

# define ft_max(x,y) ((x) > (y) ? (x) : (y))

# define TIME_FORMAT "%b %e %Y"

# define MIN_COL_WIDTH 3

# define TRUE_NAME "true"

# define FALSE_NAME "false"

# define BOOL_TO_S(b) (b ? TRUE_NAME : FALSE_NAME)

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

  /* The type of the link file if it exists */
  enum  fileType linkFileType;

  /* File information which contains the group, password, permissions time; etc */
  struct stat stat;
  /* The lt_mode of the file linked */
  mode_t  link_mode;
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
  int file_size_width;
  int file_links_width;
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
  bool params_on;
  bool print_scontext;    /* Print scontext */
  enum sorting sorting;   /* Which sorting function to use */
  enum format format;     /* Which format to use */
  enum file_indicator_type fit; /* If we need to indicate file type at the end of name */
  t_padding padding;  /* Padding info for multiple columns or horizontal format */
  bool sort_rev;            /* [-r] reverse */
  bool print_owner;         /* [-g turn OFF] default true*/
  bool print_author;        /* [--author] */
  bool print_group;         /* [-G] [-o turn OFF]*/
  bool print_block_size;    /* [-s] */
  bool print_inode;         /* [-i] */
  bool print_with_color;    /* --color=yes | no | tty */
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

// EXAMPLE MOST VERBOSE
// Inode   | block size | permissions | #links | owner        | group | size (MB) | last modified  | name
// 3918521   48          -rwxr-xr-x     1        pulgamecanica  root    47320       May  5 12:31     ft_ls


// Conf
t_conf *  init_conf(void);
void print_conf(t_conf * conf);

// Flags
bool add_flags(t_conf * conf, char * str);

// Directories
t_list ** extract_directories(t_list * list);
void  read_directories(t_file * file, t_conf * conf);
//void  read_directories(void * ptr1, void * ptr2);

// Files
t_file *  setup_file(char * name, char * path);
void  free_file(void * ptr);
void print_files(void * ptr1, void * ptr2);

// Time
char * my_ctime(time_t * time);

// Padding
void set_padding(t_list * list, t_conf * conf);
char * format_padding(char conversion, int padding, bool space, bool minus);

// Comparators
int cmp_ascii_order(void * ptr1, void * ptr2);

// Utils
int get_total_block_size(t_list * list);

#endif
