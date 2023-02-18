#include "ft_ls.h"

void	ft_exit(int status, char * msg, bool msg_allocated) {
	write(2, msg, ft_strlen(msg));
	if (msg_allocated)
		free(msg);
	exit(status);
}
