#include <stdio.h>
#include <stdlib.h>

int ft_ultimate_range(int **range, int min, int max);

int	main(void)
{
	int *array;
	int	size;
	int	i;
	int a = -50;
	int b = 210;
	size = ft_ultimate_range(&array, a, b);
	i = 0;
	while (i < size)
	{
		printf("%d, ", array[i]);
		i++;
	}
	printf("\n");
	return (0);
}
