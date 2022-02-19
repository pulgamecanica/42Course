//#include "stuff.h"
#include <stdlib.h>

#define N 4
#define TRUE 1
#define FALSE 0

int	can_place(int matrix[N][N], int vector[N], int line)
{
	int	y;
	int	i;

	y = 0;
	while (y < line)
	{
		i = 0;
		while (i < N)
		{
			if (matrix[y][i] == vector[i])
				return (FALSE);
			i++;
		}
		y++;
	}
	return (TRUE);
}

void	swap(int *x, int *y)
{
	int	temp;

	temp = *x;
	*x = *y;
	*y = temp;
}

int	factorial(int nb)
{
	if (nb > 0)
		return (nb * (factorial(nb - 1)));
	else if (nb < 0)
		return (0);
	return (1);
}

int	*generate_ref(int n)
{
	int	*array;
	int	i;

	array = (int *)malloc(sizeof (0) * n);
	if (array == NULL)
		return (0);
	i = 0;
	while (i < n)
	{
		array[i] = i + 1;
		i++;
	}
	return (array);
}

int	*p_value(int num)
{
	int	*p;
	int	temp;

	temp = num;
	p = &temp;
	return (p);
}
