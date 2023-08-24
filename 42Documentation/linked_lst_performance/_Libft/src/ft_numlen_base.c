#include <stdio.h>

int	ft_numlen_base(unsigned long int nbr, int baselen)
{
	int	total;

	total = 0;
	if (nbr == 0)
		return (1);
	while (nbr != 0)
	{
		nbr /= baselen;
		total++;
	}
	return (total);
}