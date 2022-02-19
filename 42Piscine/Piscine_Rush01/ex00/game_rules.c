#define N 4
#define TRUE 1
#define FALSE 0

int	check_pvu(int matrix[N][N], int pv[N][N])
{
	int	i;
	int	k;
	int	highest;
	int	counter;

	k = 0;
	while (k < N)
	{
		i = 0;
		highest = 0;
		counter = 0;
		while (i < N)
		{
			if (matrix[i][k] > highest)
			{
				highest = matrix[i][k];
				counter++;
			}
			i++;
		}
		if (counter != pv[0][k])
			return (FALSE);
		k++;
	}
	return (TRUE);
}

int	check_pvd(int matrix[N][N], int pv[N][N])
{
	int	i;
	int	k;
	int	highest;
	int	counter;

	k = 0;
	while (k < N)
	{
		i = 0;
		highest = 0;
		counter = 0;
		while (i < N)
		{
			if (matrix[N - i - 1][k] > highest)
			{
				highest = matrix[N - i - 1][k];
				counter++;
			}
			i++;
		}
		if (counter != pv[1][k])
			return (FALSE);
		k++;
	}
	return (TRUE);
}

int	check_pvr(int matrix[N][N], int pv[N][N])
{
	int	i;
	int	k;
	int	highest;
	int	counter;

	k = 0;
	while (k < N)
	{
		i = 0;
		highest = 0;
		counter = 0;
		while (i < N)
		{
			if (matrix[k][N - i - 1] > highest)
			{
				highest = matrix[k][N - i - 1];
				counter++;
			}
			i++;
		}
		if (counter != pv[3][k])
			return (FALSE);
		k++;
	}
	return (TRUE);
}

int	check_pvl(int matrix[N][N], int pv[N][N])
{
	int	i;
	int	k;
	int	highest;
	int	counter;

	k = 0;
	while (k < N)
	{
		i = 0;
		highest = 0;
		counter = 0;
		while (i < N)
		{
			if (matrix[k][i] > highest)
			{
				highest = matrix[k][i];
				counter++;
			}
			i++;
		}
		if (counter != pv[2][k])
			return (FALSE);
		k++;
	}
	return (TRUE);
}
