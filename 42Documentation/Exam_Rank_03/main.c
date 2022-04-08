// gcc -Wall -Werror -Wextra -o exam3 main.c

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void	putnchar(char c, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		write(1, &c, 1);
		i++;
	}
	write(1, "\n", 1);
}

void	print_matrix(char **m, int width, int height)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			write(1, &m[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}

void	put_circle(char **m, int width, int height, int xc, int yc, int rad, char c, int fill)
{
	int	i;
	int	j;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			if (fill)
			{
				if (((j - xc) * (j - xc)) + ((i - yc) * (i - yc)) <= (rad * rad))
					m[i][j] = c;
			}
			else
			{
				if (((j - xc) * (j - xc)) + ((i - yc) * (i - yc)) == (rad * rad))
					m[i][j] = c;
			}
			j++;
		}
		i++;
	}
}

int	main(int ac, char **av)
{
	FILE *f;
	char	**m;
	int	rc;
	int	height;
	int	width;
	char	bg_char;
	char	fill;
	int	x;
	int	y;
	int	radius;
	char	c;

	if (ac != 2)
	{
		write(2, "Error\n", 6);
		return (1);
	}
	putnchar('R', 10);
	putnchar('R', 10);
	f = fopen(av[1], "r");
	rc = fscanf(f, "%d %d %c\n", &height, &width, &bg_char);
	if (!rc)
	{
		printf("Error File\n");
		return (1);
	}
	printf("Read from file: Height(%d) Width(%d) and Background(%c)\n", height, width, bg_char);
	m = (char **)calloc(sizeof(char *), height);
	int i = 0;
	while (i < width)
	{
                m[i] = (char *)calloc(sizeof(char), width);
                m[i] = memset(m[i], bg_char, width);
                i++;
        }
	while((rc = fscanf(f, "%c %d %d %d %c\n", &fill, &x, &y, &radius, &c)))
	{
		if (rc == EOF)
			break;
		if (rc != 5)
		{
			printf("Wrong Format file\n");
			break;
		}
		printf("Action Founded: C(%c) X(%d) Y(%d) RADIUS(%d) CHAR(%c)\n", fill, x, y, radius, c);
		if (fill == 'c')
			put_circle(m, width, height, x, y, radius, c, 1);
		else if (fill == 'C')
			put_circle(m, width, height, x, y, radius, c, 0);
	}
//	put_circle(m, width, height, 20, 20, 5, 'c', 1);
//	put_circle(m, width, height, 40, 40, 20, 'O', 0);
	print_matrix(m, width, height);
	fclose(f);
	return (0);
}
