#include <stdio.h>
#include <stdlib.h>

char *ft_strjoin(int size, char **strs, char *sep);

int	main()
{
	char	**array;

	array = (char **)malloc(sizeof(char *) * 4);
	array[0] = "Hello";
	array[1] = "Dude!!!";
	array[2] = "We love";
	array[3] = "Pulgamecanica!!!";

	printf("Result <%s>\n", ft_strjoin(4, array, "-"));
}
