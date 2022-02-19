#include <string.h>
#include <stdio.h>

int	ft_strncmp(char *str1, char *str2, unsigned int n);

int	main(void)
{
	printf("Original vs Clone: <%d> <%d>", strncmp("bye World", "hello world", 5) , ft_strncmp("bye World", "hello world", 5));
	printf("\nOriginal vs Clone: <%d> <%d>", strncmp("hello World", "", 0) , ft_strncmp("hello World", "", 0));

}
