#define N 4
#define F_N 24
#define TRUE 1
#define FALSE 0

void	put_char(char c);
void	put_nbr(int nbr);
void	put_str(char *str);
void	swap(int *x, int *y);
void	print_matrix(int matrix[N][N]);
void	clean_matrix(int matrix[N][N], int index);
void	matrix_line(int matrix[N][N], int vector[N], int line);
int		can_place(int matrix[N][N], int vector[N], int line);
int		check_pvu(int matrix[N][N], int pv[N][N]);
int		check_pvd(int matrix[N][N], int pv[N][N]);
int		check_pvr(int matrix[N][N], int pv[N][N]);
int		check_pvl(int matrix[N][N], int pv[N][N]);
int		get_col(int i);
int		get_line(int i);
int		factorial(int nb);
int		*p_value(int num);
int		*generate_ref(int n);

int	g_todo = TRUE;

int	is_solution(int matrix[N][N], int pv[N][N])
{
	return (check_pvu(matrix, pv)
		&& check_pvd(matrix, pv)
		&& check_pvr(matrix, pv)
		&& check_pvl(matrix, pv));
}

void	next_recursive(int m[N][N], int combs[F_N][N], int index, int pv[N][N])
{
	int	k;

	if (index + 1 == N)
	{
		if (is_solution(m, pv))
		{
			print_matrix(m);
			g_todo = FALSE;
		}
	}
	else if (index < N)
	{
		k = 0;
		while (k < F_N && g_todo)
		{
			if (can_place(m, combs[k], index + 1))
			{
				matrix_line(m, combs[k], index + 1);
				next_recursive(m, combs, index + 1, pv);
			}
			k++;
		}
	}
}

void	heap_combs(int matrix[F_N][N], int ref[N], int n, int *line)
{
	int	i;

	if (n == 1)
	{
		matrix_line(matrix, ref, *line);
		*line += 1;
	}
	else
	{
		i = 0;
		while (i < n)
		{
			heap_combs(matrix, ref, n - 1, line);
			if (n % 2 == 1)
				swap(&ref[0], &ref[n - 1]);
			else
				swap(&ref[i], &ref[n - 1]);
			i++;
		}
	}
}

void	solve(int pv[N][N])
{
	int	combs[F_N][N];
	int	matrix[N][N];
	int	i;

	heap_combs(combs, generate_ref(N), N, p_value(0));
	clean_matrix(matrix, 0);
	i = 0;
	while (i < factorial(N) && g_todo)
	{
		matrix_line(matrix, combs[i], 0);
		next_recursive(matrix, combs, 0, pv);
		i++;
	}
}

int	main(int ac, char **av)
{
	int	pv[N][N];
	int	i;
	int	j;

	i = 0;
	if (ac == 2)
	{
		i = 0;
		j = 0;
		while (av[1][j] != '\0')
		{
			while (av[1][j] > '0' + N || av[1][j] < '0')
				j++;
			pv[get_line(i)][get_col(i)] = (int)av[1][j] - 48;
			i++;
			j++;
		}
		if (i != (N * N))
			put_str("ERROR!");
		else
			solve(pv);
	}
	put_char('\n');
	return (1);
}
