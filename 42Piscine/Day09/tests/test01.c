#include <stdio.h>

int *ft_range(int min, int max);

int	main()
{
	printf("%d", ft_range(-1, 7)[0]);
	printf("%d", ft_range(-1, 7)[1]);
	printf("%d", ft_range(-1, 7)[2]);
	printf("%d", ft_range(-1, 7)[3]);
	printf("%d", ft_range(-1, 7)[4]);
	printf("%d", ft_range(-1, 7)[5]);
	printf("%d", ft_range(-1, 7)[6]);
	printf("%d", ft_range(-1, 7)[7]);
	printf("\n");
	return (0);
}
