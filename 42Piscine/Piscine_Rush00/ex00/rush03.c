/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rush03.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 11:51:10 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/12 11:51:12 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_putchar(char c);
void	ft_put_n_char(int n, char c, char start_c, char end_c);
void	rush(int x, int y);

void	ft_put_n_char(int n, char c, char start_c, char end_c)
{
	int	i;

	i = 0;
	ft_putchar(start_c);
	while (i < n)
	{
		ft_putchar(c);
		i++;
	}
	if (n >= 0)
		ft_putchar(end_c);
}

void	rush(int x, int y)
{
	int	i;

	i = 0;
	while ((i < y) && (x > 0))
	{
		if (i == 0)
			ft_put_n_char(x - 2, 'B', 'A', 'C');
		else if (i == y - 1)
			ft_put_n_char(x - 2, 'B', 'A', 'C');
		else
			ft_put_n_char(x - 2, ' ', 'B', 'B');
		ft_putchar('\n');
		i++;
	}
}
