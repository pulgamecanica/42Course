	#include <stdio.h>
#include <stdlib.h>

char    *ft_convert_base(char *nbr, char *base_from, char *base_to);

int	main()
{
	printf("Result: <%s>\n", ft_convert_base("   -+--andre", "andre", "0123456789"));
	printf("Result: <%s>\n", ft_convert_base("   -0001233", "012345", "01234"));
	printf("Result: <%s>\n", ft_convert_base("   -+-0", "0123456789ABCDEF", "0123456789"));
	printf("Result: <%s>\n", ft_convert_base("   -+-ane", "andre0123433", "0123456789abcdef"));
	printf("Result: <%s>\n", ft_convert_base("   -+-ane", "andre", "0123456789abcdef"));
	printf("Result: <%s>\n", ft_convert_base("   -+-----1235513", "012345678", "0123456789"));
	printf("Result: <%s>\n", ft_convert_base("   -+-ane", "andre", "0123456789abcdef"));
	printf("Result: <%s>\n", ft_convert_base("   -+hahahaha", "012345ha", "lofi"));
	printf("Result: <%s>\n", ft_convert_base("   -+---1010000100111110", "01", "0123456789"));
}
