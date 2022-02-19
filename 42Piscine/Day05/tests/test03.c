#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char	*ft_strncat(char *dest, char *src, size_t n);

int	main(void)
{
	char	*s1;
	char	*s2;

	s1 = strdup("Hello World");
	s2 = strdup("Hello World");
	printf("Original vs Clone: <%s> - <%s>\n", strncat(s1, "hello world", 4), ft_strncat(s2, "hello world", 4));
	free(s1);
	free(s2);
	s1 = strdup("");
	s2 = strdup("");
	printf("Original vs Clone: <%s> - <%s>\n", strncat(s1, "", 10) , ft_strncat(s1, "", 10));
	return (0);
}
