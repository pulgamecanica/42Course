#include <string.h>
#include <stdio.h>

size_t	ft_strlcat(char *dst, char *src, size_t size);

int	main(void)
{
	char	*s1;
	char	*s2;

	s1 = strdup("Everything is Awesome");
	s2 = strdup("");
	printf("Original vs Clone: <%ld (%s)> <%ld (%s)>\n", ft_strlcat(s1, s2, 15), s1, ft_strlcat(s1, s2, 15), s1);
	return (0);
}
