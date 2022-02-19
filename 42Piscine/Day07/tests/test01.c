#include <stdio.h>
int ft_recursive_factorial(int nb);

int	main(void)
{
	int	nb;

	nb = 10;
	printf("Result of %d! is: %d\n", nb, ft_recursive_factorial(nb));
	nb = 0;
	printf("Result of %d! is: %d\n", nb, ft_recursive_factorial(nb));
	nb = 112;
	printf("Result of %d! is: %d\n", nb, ft_recursive_factorial(nb));
	nb = 10;
	printf("Result of %d! is: %d\n", nb, ft_recursive_factorial(nb));
	nb = -97;
	printf("Result of %d! is: %d\n", nb, ft_recursive_factorial(nb));
	nb = 1;
	printf("Result of %d! is: %d\n", nb, ft_recursive_factorial(nb));
}
