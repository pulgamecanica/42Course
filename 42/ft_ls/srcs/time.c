#include "ft_ls.h"

/**
	* ctime will return the time in the format:
	*	"Wed Jun 30 21:49:08 1993\n"
	*	[0123456789012345678901234] --> INDEX
	*	 0---------10--------20
	* The ls program only shows:
	*	"Month day minutes:seconds"
	* That is why I copy from the index 4 to the index 16
	**/
char * my_ctime(time_t * time) {
	static char my_time[] = "Jan 00 00:00";

	if (time)
		ft_strlcpy(my_time, ctime(time) + 4, 13);
	return (my_time);
}