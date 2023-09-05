#ifndef __CONF__H__
#define __CONF__H__

# include <stdbool.h>

# define ft_max(x,y) ((x) > (y) ? (x) : (y))

enum format
{
	one_per_line,
	multiple_in_column,
	multiple_in_row,
	long_format,
	coma_separated
};

enum sorting
{
	no_sorting,
	by_time,
	by_name,
	by_size,
	by_ext
};

typedef struct s_long_options {
	bool owner;
	bool author;
	bool group;
	// short int bytes_per_block;
	bool numeric_uid_gid;
}	t_long_options;


typedef struct s_padding {
	short int inode;
	short int block_size;
	short int size;
	// short int scontext;
	short int owner;
	short int group;
	short int author;
	short int major_device;
	short int minor_device;
	short int links;
	// short int file_width;
}	t_padding;

typedef struct s_conf {
	void *	format_f;
	void *	sorting_f;

	t_padding padding;
	/**
	 * -l long listing
	 * -1 one per line (default)
	 * -x multiple ordered by row
	 * -m comma separated
	 **/
	enum format format;

	/**
	 * no option = sorting by name (default)
	 * -t time
	 * -X extension alphabetically
	 * -U do not sort
	 * -r reverse order
	 **/
	enum sorting sorting;

	/* Will hold the long options when long format is on*/
	t_long_options l_opts;
	
	/* --color=[always, auto, never] */
	bool color;
	
	/* -s --size */
	bool block_size;
	/* -i --inode */
	bool inode;
	/* -r --recursive */
	bool sort_rev;
	/* -a --all */
	bool no_ignore;
	/* -A --almost-all */
	bool almost_no_ignore;
	/* -d --directory */
	bool no_explore;
	/* -R --recursive */
	bool recursive;
	/* -- Will deactivate options after expression */
	bool delimit;
	/* -p --indicator-style=slash */
	bool print_indicator;
	/* need_stat when long option is on only */
	bool need_stat;
	/* Print Directory before listing */
	bool print_dir;
	/* If if the directory listing needs a preceding separation (new line)*/
	bool initial_separation;
}	t_conf;


t_conf * new_default_conf_ls();

#endif