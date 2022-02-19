#include <stdio.h>

int ft_fibonacci(int index);

int	main(void)
{
	int	index = 10;
	while (index-- >= 0)
		printf("Index %d: <%d>\n", index, ft_fibonacci(index));
}
