#include <unistd.h>

void	put_char(char c)
{
	write(1, &c, 1);
}

void	put_str(char *str)
{
	while (*str != '\0')
	{
		put_char(*str++);
	}
}

void	put_nbr(int nbr)
{
	if (nbr < 0)
	{
		if (nbr == -2147483648)
		{
			put_nbr(-2147);
			put_nbr(483648);
			return ;
		}
		put_char('-');
		nbr = -nbr;
	}
	if (nbr > 9)
		put_nbr(nbr / 10);
	put_char(nbr % 10 + 48);
}
