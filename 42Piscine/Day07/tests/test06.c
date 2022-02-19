#include <stdio.h>

int ft_is_prime(int nb);

int	main(void)
{
	int	nb;

	nb = -1;
	while (nb++ != 20)
		printf("Result of %d! is: %d\n", nb, ft_is_prime(nb));
}
