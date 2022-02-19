#include "push_swap.h"

int	check_repeats(t_stack *stack, int checker)
{
	t_stack *temp;
	t_stack *temp2;

	temp = stack;
	while (temp)
	{
		temp2 = stack;
		while (temp != temp2)
		{
			if ((int)temp->content == (int)temp2->content)
			{
				if (!checker)
					ft_putstr_fd("Error\n", 2);
				return (0);
			}
			temp2 = temp2->next;
		}
		temp = temp->next;
	}
	return (1);
}


int	check_errors(char *str, int checker)
{
	int i;

	i = 0;
	if (str[0] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i++]))
		{
			if (!checker)
				ft_putstr_fd("Error\n", 2);
			return (-1);
		}
	}
	if (ft_atoi(str) > 2147483647 || ft_atoi(str) < -2147483648)
	{
		if (!checker)
			ft_putstr_fd("Error\n", 2);
		return (-1);
	}
	return (0);	
}
