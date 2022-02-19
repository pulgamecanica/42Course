/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ten_queens_puzzle.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 01:35:42 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/18 01:35:44 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>

void	put_sec(int board[10][10])
{
	int		column;
	int		line;
	char	c;

	column = 0;
	while (column <= 9)
	{
		line = 0;
		while (line <= 9)
		{
			if (board[line][column] == 5)
			{
				c = ((char)line) + '0';
				write(1, &c, 1);
			}
			line++;
		}
		column++;
	}
	write(1, "\n", 1);
}

int	can_place_queen(int board[10][10], int line, int column)
{
	int	i;

	i = 0;
	while (i < 10)
	{	
		if (board[line][i] == 5 || board[i][column] == 5)
			return (0);
		if (line + i < 10 && column + i < 10)
			if (board[line + i][column + i] == 5)
				return (0);
		if (line + i < 10 && column - i >= 0)
			if (board[line + i][column - i] == 5)
				return (0);
		if (line - i >= 0 && column + i < 10)
			if (board[line - i][column + i] == 5)
				return (0);
		if (line - i >= 0 && column - i >= 0)
			if (board[line - i][column - i] == 5)
				return (0);
		i++;
	}
	return (1);
}

void	clear_board(int board[10][10])
{
	int	i;
	int	j;

	i = 0;
	while (i < 10)
	{
		j = 0;
		while (j < 10)
		{
			board[i][j] = 0;
			j++;
		}
		i++;
	}
}

void	solve(int board[10][10], int column, int *res)
{
	int	line;

	line = 0;
	if (column > 9)
	{
		(*res)++;
		put_sec(board);
		return ;
	}
	while (line < 10)
	{
		if (can_place_queen(board, line, column))
		{
			board[line][column] = 5;
			solve(board, column + 1, res);
		}
		board[line][column] = 0;
		line++;
	}
}

int	ft_ten_queens_puzzle(void)
{
	int	board[10][10];
	int	solutions;

	solutions = 0;
	clear_board(board);
	solve(board, 0, &solutions);
	return (solutions);
}
