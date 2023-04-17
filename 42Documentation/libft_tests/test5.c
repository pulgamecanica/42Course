#include <stdio.h>
#include <stdlib.h>

#define DEBUG
#define malloc(x) NULL

char ** ft_split(char *str, char sep);
void	ft_putstr_fd(char *, int);

void	print_list(char **list) {
	if (!list || !(*list))
		return ;
	printf("[%s]\n", *list);
	print_list(++list);
}


void free_list(char **list) {
	int i;
	if (!list)
		return ;
	i = 0;
	while (list[i])
		free(list[i++]);
	free(list);
}

int main (int ac, char *av[])
{
	int sep;
	char **tmp_list;
	char * err;

	if (ac < 3) {
		ft_putstr_fd("Usage: ./test separator string1 string2 string...\n", 2);
		return EXIT_FAILURE;
	}
	sep = (int)av[1][0];
	err = NULL;
	*err = 'c';
	for (int i = 2; i < ac; i++)
	{
		tmp_list = ft_split(av[i], sep);
		print_list(tmp_list);
		free_list(tmp_list);
	}
	printf("Testing Finished\n");

	return EXIT_SUCCESS
;
}
