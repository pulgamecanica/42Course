#include "../libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void	original_mem(char *str)
{
	printf("*********************************************\n");
	memset(str,'$',7);
        printf("memset(7)[$]: %s\n", str);
        memcpy(str, "Hello World", 11);
        printf("memcpy(11)[Hello World]: %s\n", str);
        memmove(str, "Good Day", 5);
        printf("memove(20)[Good Day]: %s\n", str);
        printf("memchr(a)[%s]: %p\n", str, memchr(str, 97, 8));
	char	*s1 = "Hell";
	printf("memcmp[Hell][Hello]: %d\n", memcmp("Hello", s1, 5));
	bzero(str, 10);
	printf("bzero(10): %s\n", str);
	printf("*********************************************\n");
}
void	mem_tests(char *str)
{
	original_mem(ft_strdup(str));
	str = ft_strdup(str);
	ft_memset(str,'$',7);
	printf("ft_memset(7)[$]: %s\n", str);
	ft_memcpy(str, "Hello World", 11);
	printf("ft_memcpy(11)[Hello World]: %s\n", str);
	ft_memmove(str, "Good Day", 5);
	printf("ft_memove(20)[Good Day]: %s\n", str);
	printf("ft_memchr(a)[%s]: %p\n", str, ft_memchr(str, 97, 8));
	char    *s1 = "Hell";
	printf("ft_memcmp[Hell][Hello]: %d\n", ft_memcmp("Hello", s1, 5));
	ft_bzero(str, 10);
	printf("ft_bzero(10): %s\n", str);
	printf("*********************************************");
}

void	srtchar_tests(char *str)
{
	char	test = '\0';
	printf("ft_strchr[0]: %s\n", ft_strchr(str, (int)test));
	printf("strchr[0]: %s\n", strchr(str, (int)test));
	printf("ft_strrchr[a]: %s\n", ft_strrchr(str, 97));
	printf("strrchr[a]: %s\n", strrchr(str, 97));

}

void	strn_tests(char *str)
{
	printf("ft_strncmp(6)[pulga & %s]: %d\n", str, ft_strncmp("pulga", str, 4));
	printf("strncmp(6)[pulga & %s]: %d\n", str, strncmp("pulga", str, 4));
	//printf("ft_strnstr(14)[mecanica]: %s\n", ft_strnstr(str, "mecanica", 14) != 0 ? "Founded" : "Not Founded");
	//printf("strnstr(14)[mecanica]: %s\n", strnstr(str, "mecanica", 14) != 0 ? "Founded" : "Not Founded");
	printf("ft_strncmp: %d\n", ft_strncmp("test\200", "test\0", 6));
	printf("strncmp: %d\n", strncmp("test\200", "test\0", 6));
}

void	is_tests(int	c)
{
	printf("isalpha: %d\n", ft_isalpha(c));
	printf("isdigit: %d\n", ft_isdigit(c));
	printf("isalnum: %d\n", ft_isalnum(c));
	printf("isascii: %d\n", ft_isascii(c));
	printf("isprint: %d\n", ft_isprint(c));
}

void	strl_tests(char *str)
{
	char	temp[10];
//	char	temp2[10];

	printf("Length: %zu\n", ft_strlen(str));
	ft_strlcpy(temp, str, 4);
	printf("ft_strlcpy(4): %s\n", temp);
	//strlcpy(temp2, str, 4);
	//printf("strlcpy(4): %s\n", temp2);
	ft_strlcat(temp, "PULGA", 5);
	printf("ft_strlcat(5)[PULGA]: %s\n", temp);
	//strlcat(temp2, "PULGA", 5);
	//printf("strlcat(5)[PULGA]: %s\n", temp2);
}

void	to_smthng_tests(int c)
{
	printf("Lower: %c\n", ft_tolower(c));
	printf("Upper: %c\n", ft_toupper(c));
}

void	split_tests(char const *str, char c)
{
	char	**array;
	int	i;

	i = 0;
	array = ft_split(str, c);
	printf("\nft_split[%c]:", c);
	while (array[i] != 0)
	{
		printf("\n	#%d -> %s", i, array[i]);
		i++;
	}
}

char    odd_chars(unsigned int i, char c)
{
        if (i % 2 != 0)
                return (c);
        return ('9');
}

char	even_chars(unsigned int i, char c)
{
	if (i % 2 == 0)
		return (c);
	return ('6');
}
void	convert_up(unsigned int i, char *str)
{
	str[i] = ft_toupper(str[i]);
}

void	strmapi_tests(char *str)
{
	void	*f;

	f = even_chars;
	printf("ft_strmapi(even_chars(6)): %s\n", ft_strmapi(str, f));
	printf("ft_strmapi(odd_chars(9)): %s\n", ft_strmapi(str, &odd_chars));
}

void    striteri_tests(char *str)
{
        void    *f;

        f = convert_up;
	ft_striteri(str, f);
        printf("ft_striteri(convert_up): %s\n", str);
}

void 	putstuff_tests()
{
	ft_putstr_fd("Hello Worldº	wwww\n", 1);
	ft_putendl_fd("New_line->", 1);
	ft_putnbr_fd(-2147483648, 1);
	ft_putendl_fd("", 1);
	ft_putnbr_fd(2147483647, 1);
	ft_putendl_fd("", 1);
	ft_putnbr_fd(99999, 1);
	ft_putendl_fd("", 1);
	ft_putnbr_fd(0, 2);
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		//av[1] = ft_strdup(" ");
		is_tests(av[1][0]);
		strl_tests(av[1]);
		to_smthng_tests(av[1][0]);
		strn_tests(av[1]);
		srtchar_tests(av[1]);
		printf("ft_atoi: %d\n", ft_atoi(av[1]));
		printf("atoi: %d\n", atoi(av[1]));
		mem_tests(av[1]);
		printf("\nft_substr(6, 20): %s", ft_substr("Hello World", 6, 20));
		printf("\nft_substr(0, 0): %s", ft_substr("Hello World", 0, 0));
		printf("\nft_substr(20, 4): %s", ft_substr("Hello World", 20, 4));
		printf("\nft_strjoin(pulgamecanica): %s", ft_strjoin("I love", " pulgamecanica"));
		split_tests(av[1], ' ');
		split_tests(av[1], ',');
		split_tests(av[1], '\n');
		printf("\nft_itoa(-213331): %s\n", ft_itoa(-213331));
		printf("ft_itoa(-10): %s\n", ft_itoa(-10));
		printf("ft_itoa(-2147483648): %s\n", ft_itoa(-2147483648));
		printf("ft_itoa(2147483647): %s\n", ft_itoa(2147483647));
		strmapi_tests(av[1]);
		printf("TRIM: %s\n", ft_strtrim("tripouille   xxx", " x"));
		printf("ft_strtrim( ,): %s\n", ft_strtrim(av[1], " ,"));
		striteri_tests(av[1]);
		putstuff_tests();
	}
	else
		printf("Usage: ./test <one_arg>");
	printf("\n");
}
