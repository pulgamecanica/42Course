#include "get_next_line.h"
#include <stdio.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*str1;
	const unsigned char	*str2;

	str1 = s1;
	str2 = s2;
	while (n--)
	{
		if (*str1++ != *str2++)
			return (*--str1 - *--str2);
	}
	return (0);
}


char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	unsigned int	i;

	i = ft_strlen(needle);
	if (i == 0)
		return ((char *)haystack);
	while (len >= i && ft_strlen(haystack) != 0)
	{
		len--;
		if (!ft_memcmp(haystack, needle, i))
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}


static char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*new;

	if (s == NULL)
		return (NULL);
	if (len > ft_strlen(s))
		len = ft_strlen(s);
	new = (char *)malloc(sizeof(char) * (len + 1));
	if (new == NULL)
		return (NULL);
	i = 0;
	if (ft_strlen(s) > start)
	{
		while (i < len && s[start + i] != '\0')
		{
			new[i] = s[start + i];
			i++;
		}
	}
	new[i] = '\0';
	return (new);
}


static char	*ft_strdup(const char *src)
{
	int		n;
	char	*cpy;

	cpy = (char *)malloc(sizeof(*src) * ft_strlen(src) + 1);
	if (cpy == NULL)
		return (NULL);
	n = 0;
	while (src[n] != '\0')
	{	
		cpy[n] = src[n];
		n++;
	}
	cpy[n] = '\0';
	return (cpy);
}


static char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	i;
	unsigned int	k;

	if (ft_strlen(s1) <= 0)
		return (ft_strdup(""));
	i = 0;
	k = ft_strlen(s1) - 1;
	while (ft_strchr(set, s1[i]) != NULL)
	{
		if (i++ > k)
			return (ft_strdup(""));
	}
	while (ft_strchr(set, s1[k]) != NULL)
	{
		if (k-- <= i)
			return (ft_strdup(""));
	}
	return (ft_substr(s1, i, k + 1 - i));
}


void	main_menu()
{
	printf("		====>MAIN MENU<====\n");
	printf("	1) Play Game\n");
	printf("	2) Free Text\n");
	printf("	3) Read File\n");
	printf("	0) EXIT\n");
}

void	game_menu()
{
	printf("\033[0;0H\033[2J\n");
	printf("		====>GAME<====\n");
	printf("	1) Say Hello\n");
	printf("	2) Talk to Mama\n");
	printf("	3) Talk to Papa\n");
	printf("	4) Talk to Granny!\n");
	printf("	5) Kill'em all...\n");
	printf("	6) Show MENU\n");
	printf("	0) EXIT GAME\n");
}

void	start_game()
{
	char	*line;
	int	granny_count = 0;

	game_menu();
	while ((line = get_next_line(0)))
	{
		line = ft_strtrim(line, "       ");
		if (*line == '1')
		{		
			printf("Hello to you toooo!\n");
		}
		else if (*line == '2')
		{
			if (granny_count)
			{
				printf("HOW DARE YOU YELL AT YOUR GRANNY! DONT TALK TO ME!\n");
				continue ;
			}
			else
			{
				printf("Dear, what do you want?\n");
				line = get_next_line(0);
				if (ft_strnstr(line, "love", ft_strlen(line)))
					printf("I love you too, honey!\n");
				else
					printf("So you don't love ME!? :(\n");
					
			}
		}
		else if (*line == '3')
			if (granny_count)
                        {
                                printf("You LITTLE RAT! Why u scream to grand'ma!!!\n");
                                continue ;
                        }
                        else
                        {
                                printf("Tell me son... what's on your mind?\n");
                                line = get_next_line(0);
                                if (ft_strnstr(line, "fuck", ft_strlen(line)))
                                        printf("AHHHHHH you little piece of shit come here right now!!! I'll smack your ass!\n");
                                else
                                        printf("So you don't love ME!? :(\n");

                        }
		else if (*line == '4')
		{
			printf("Hello my sweet, here's your granny, ask my anything...\n");
			line = get_next_line(0);
			if (ft_strnstr(line, "!!!", ft_strlen(line)))
			{
				printf("Ahhhhhhh don't scream at me! what did I do to you, you damn bastard!\n");
				granny_count++;
			}
			else
				printf("Hugh!? What did you say!? No no since 1998!\n");
		}
		else if (*line == '5')
		{
			printf("Done.... how sad.... bye bye\n");
			break ;
		}
		else if (*line == '6')
			game_menu();
		else if (*line == '0')
			break ;
		else
			printf("Sorry that's not a valid option... :(\n");
	}
}

void read_file() {
	int fd;
	char * tmp;
	char * filename;
	int ok;

	printf("Please enter the filename:\n");
	filename = get_next_line(0);
	filename = ft_strtrim(filename, " 	\n");
	fd = open(filename, O_RDONLY);
	if (fd <= 0) {
		printf("Sorry couldn't open \"%s\"\n", filename);
	} else {
		do {
			ok = 1;
			tmp = get_next_line(fd);
			if (!tmp)
				ok = 0;
			else
				printf("%s", tmp);
			free(tmp);
		} while (ok);
	}
	printf("Press ENTER to return to the Menu ;)\n");
	free(get_next_line(0));
}

int free_text() {
	char *tmp;

	tmp = get_next_line(0);
	if (!tmp)
		return NULL;
	printf("[%lld]bytes read: %s", ft_strlen(tmp), tmp);
	if (ft_strlen(tmp) > 1) {
		free(tmp);
		return 1;
	}
	free(tmp);
	return 0;
}

int	main(void)
{
	char	*line;

	main_menu();
	while ((line = get_next_line(0)))
	{
		line = ft_strtrim(line, " 	\n");
		if (*line == '1')
			start_game();
		else if (*line == '2')
			while (free_text());
		else if (*line == '3')
			read_file();
		else if (*line == '0')
			break ;
		else
			printf("Sorry that's not a valid option... :(\n");
		printf("\033[0;0H\033[2J\n");
		printf("Option choosen: %s\n", line);
		main_menu();
		free(line);
	}
}
