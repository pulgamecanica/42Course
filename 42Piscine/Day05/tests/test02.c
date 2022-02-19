#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char	*ft_strcat(char *dest, char *src);

int	main(void)
{
	char	*s1;
	char	*s2;

	s1 = strdup("Hello World");
	s2 = strdup("Hello World");
	printf("Original vs Clone: <%s> - <%s>\n", strcat(s1, "hello world"), ft_strcat(s2, "hello world"));
	free(s1);
	free(s2);
	s1 = strdup("");
	s2 = strdup("");
	printf("Original vs Clone: <%s> - <%s>\n", strcat(s1, "") , ft_strcat(s1, ""));
	return (0);
}
