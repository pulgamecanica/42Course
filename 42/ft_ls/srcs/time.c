#include "ft_ls.h"

char * my_ctime(time_t * time) {
	static char my_time[] = "Jan 00 00:00";

	if (time)
		ft_strlcpy(my_time, ctime(time) + 4, 13);
	return (my_time);
}