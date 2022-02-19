#include <stdio.h>

int ft_atoi_base(char *str, char *base);

int	main(void)
{
	printf("%d\n", ft_atoi_base(" ---+-abc + 893 --", "0123456789abcdef"));
        printf("%d\n", ft_atoi_base("  ---+-223bc + 893 ---", "012345"));
        printf("%d\n", ft_atoi_base("  ----+-123 + 893 --", "0123"));
        printf("%d\n", ft_atoi_base(" ---+-15234abc + 893 --", "0123456789"));
        printf("%d\n", ft_atoi_base("  ----+-15 + 893 --", "01"));
        printf("%d\n", ft_atoi_base("  ----+-122 + 893 --", "012345678"));
        printf("%d\n", ft_atoi_base("  -----+-andre + 893 --", "andre"));
}
