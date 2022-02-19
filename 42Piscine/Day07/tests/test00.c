#include <stdio.h>
int ft_iterative_factorial(int nb);

int	main(void)
{
	int	nb;

	nb = 10;
	printf("Result of %d! is: %d", nb, ft_iterative_factorial(nb));
}
