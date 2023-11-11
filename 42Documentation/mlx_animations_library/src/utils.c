#include <sys/time.h>
#include "get_next_line.h"
#include "utils.h"
#include "libft.h"

int		ft_list_size(char ** list) {
	int i;

	if (!list || !*list)
		return 0;
	i = 0;
	while (list[i])
		i++;
	return i;
}

void	free_list(char ** list) {
	int	i;

	if (!list)
		return ;
	i = 0;
	while(list[i]) {
		free(list[i]);
		i++;
	}
	free(list);
}

char * get_next_line_nl(int fd) {
	char *	line;
	char *	new_line;

	line = get_next_line(fd);
	if (!line) {
		close(fd);
		return NULL;
	}
	if (line[ft_strlen(line) - 1] != '\n')
		return line;
	if (ft_strlen(line) <= 1) {
		free(line);
		return ft_strdup("");
	}
	new_line = (char *)ft_calloc(sizeof(char), ft_strlen(line));
	ft_strlcpy(new_line, line, ft_strlen(line));
	free(line);
	return (new_line);
}
