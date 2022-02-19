#include <stdio.h>
int ft_iterative_power(int nb, int power);

int	main(void)
{
	int	a;
	int	b;
	a = 2;
	b = 9;
	printf("%d**%d: <%d>\n", a, b, ft_iterative_power(a, b));
	a = 0;
	b = 0;
	printf("%d**%d: <%d>\n", a, b, ft_iterative_power(a, b));
	a = 3585;
	b = -123334;
	printf("%d**%d: <%d>\n", a, b, ft_iterative_power(a, b));
	a = -1234;
	b = -12;
	printf("%d**%d: <%d>\n", a, b, ft_iterative_power(a, b));
	a = -12;
	b = 23;
	printf("%d**%d: <%d>\n", a, b, ft_iterative_power(a, b));
	a = -213;
	a = 2;
	printf("%d**%d: <%d>\n", a, b, ft_iterative_power(a, b));
}
