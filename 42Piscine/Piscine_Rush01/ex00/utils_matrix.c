#define N 4

void	put_char(char c);
void	put_nbr(int nbr);
void	print_matrix(int matrix[N][N]);

int	get_col(int index)
{
	if (index >= 0 && index < (N * N))
		return (index % N);
	return (-1);
}

int	get_line(int index)
{
	if (index >= 0 && index < (N * N))
		return (index / N);
	return (-1);
}

void	print_matrix(int matrix[N][N])
{
	int	i;
	int	j;

	i = 0;
	while (i < N)
	{
		j = 0;
		while (j < N)
		{
			put_nbr(matrix[i][j]);
			if (j != N - 1)
				put_char(' ');
			j++;
		}
		if (i != N - 1)
			put_char('\n');
		i++;
	}
}

void	clean_matrix(int matrix[N][N], int index)
{
	int	i;

	i = 0;
	if (index + 1 != N)
	{
		while (i < N)
			matrix[index][i++] = 0;
		clean_matrix(matrix, index + 1);
	}
}

void	matrix_line(int matrix[][N], int vector[N], int line)
{
	int	i;

	i = 0;
	while (i < N)
	{
		matrix[line][i] = vector[i];
		i++;
	}
}
