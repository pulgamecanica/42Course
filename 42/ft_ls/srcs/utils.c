#include "ft_ls.h"

void	ft_exit(int status, char * msg, bool msg_allocated) {
	write(2, msg, ft_strlen(msg));
	if (msg_allocated)
		free(msg);
	exit(status);
}

char * get_file_time(t_file * file, ls_flags * flags) {
	char * time_str;
	char * str;

	if (!flags || !file || file->f_errors != NoError)
		return NULL;
	time_str = NULL;
	if (flags->flagt) {
		time_str = ctime(&file->f_stat.st_mtime);
	} else {
		time_str = ctime(&file->f_stat.st_ctime);
	}
	str = (char *)ft_calloc(sizeof(char), ft_strlen(time_str));
	if (!str)
		return NULL;
	ft_strlcpy(str, time_str, ft_strlen(time_str) - 1);
	return str;
}