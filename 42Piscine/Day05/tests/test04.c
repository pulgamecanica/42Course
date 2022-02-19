#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char	*ft_strstr(const char *haystack, const char *needle);

int	main(void)
{
	char	*s1;

	s1 = strdup("");
	printf("Original vs Clone: <%s> <%s>\n", strstr("hello World", s1) , ft_strstr("hello World", s1));
	free(s1);
	s1 = strdup("Find Me");
	printf("Original vs Clone: <%s> <%s>\n", strstr("Can you Find Me?", s1) , ft_strstr("Can you Find Me?", s1));
	printf("Original vs Clone: <%s> <%s>\n", strstr("Find Me", s1) , ft_strstr("Find Me", s1));
	printf("Original vs Clone: <%s> <%s>\n", strstr("", "") , ft_strstr("", ""));
	free(s1);
	return (0);
}
