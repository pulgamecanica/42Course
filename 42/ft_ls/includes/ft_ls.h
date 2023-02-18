#ifndef __FT_LS__
#define __FT_LS__

# include "libft.h"
# include "ft_printf.h"
# include <stdbool.h>

typedef struct ls_flags {
	bool flagl; // -l
	bool flagR; // -R --recursive
	bool flaga; // -a --all
	bool flagr; // -r --reverse
	bool flagt; // -t
}	ls_flags;

typedef struct ls_config {
	ls_flags	flags;
	t_list		head;
}	ls_config;

#define ft_max(x,y) ((x) > (y) ? (x) : (y))

#endif
