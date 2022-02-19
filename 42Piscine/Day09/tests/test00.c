#include <stdio.h>
char *ft_strdup(char *src);

int	main(void)
{
	char *src = ft_strdup("Hello World123!!!");
	printf("Copy: %s\n", src);
	return (0);
}
