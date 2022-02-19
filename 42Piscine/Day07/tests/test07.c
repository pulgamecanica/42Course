#include <stdio.h>

int ft_find_next_prime(int nb);

int	main(void)
{
	int	nb;

	nb = 2147483647;
	printf("Result of %d! is: %d\n", nb, ft_find_next_prime(nb));
	nb = 2147483645;
	printf("Result of %d! is: %d\n", nb, ft_find_next_prime(nb));
	nb = 31273462;
	printf("Result of %d! is: %d\n", nb, ft_find_next_prime(nb));
	nb = 47934;
	printf("Result of %d! is: %d\n", nb, ft_find_next_prime(nb));

}
