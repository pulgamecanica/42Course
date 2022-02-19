#include <stdio.h>

void    ft_putnbr_base(int nbr, char *base);

int	main(void)
{
	int	n = 12;
	char	*b = "1234";
	ft_putnbr_base(n, b);
	printf("\n");
	n = -1241;
	b = "012345678";
	ft_putnbr_base(n, b);
	printf("\n");
	n = 12;
	b = "0123456789abcde";
	ft_putnbr_base(n, b);
	printf("\n");
	n = 432;
	b = "andre";
	ft_putnbr_base(n, b);
	printf("\n");
	ft_putnbr_base(12, "013");
}
