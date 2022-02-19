#include <stdio.h>
int ft_sqrt(int nb);

int	main(void)
{
	int	nb;

	nb = 0;
	printf("Result of %d! is: %d\n", nb, ft_sqrt(nb));
	nb = -1;
	printf("Result of %d! is: %d\n", nb, ft_sqrt(nb));
	nb = 2147483647;
	printf("Result of %d! is: %d\n", nb, ft_sqrt(nb));
}
