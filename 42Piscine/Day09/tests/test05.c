#include <stdio.h>
#include <stdlib.h>

char **ft_split(char *str, char charset);

int	main()
{
	char	**array;
	array = ft_split("~/Hello ~/World  ... ~Im ~/ All Good! :D~/dddd~/JAJAJ~/", '/');
	while (*array != 0)
		printf("String: %s\n", *(array++));
}
