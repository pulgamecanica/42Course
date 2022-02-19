#include <stdio.h>

int ft_ten_queens_puzzle(void);

int	main(void)
{
	int solutions;
	solutions = ft_ten_queens_puzzle();
	printf("Total Solutions: %d", solutions);
}

/*
void	print_board(int board[10][10])
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (i < 10)
	{
		j = 0;
		while (j < 10)
		{
			c = (board[i][j++]) + '0';
			write(1, " ", 1);
			write(1, &c, 1);
			write(1, " ", 1);
		}
		write(1, "\n", 1);
		i++;
	}
	write(1, "\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n", 32);
}
*/
