/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 01:01:25 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/18 01:01:27 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_sqrt(int nb)
{
	unsigned int	i;

	if (nb <= 0)
		return (0);
	if (nb < 2)
		return (1);
	i = 0;
	while (1)
	{
		if (i * i > (unsigned int)nb)
			return (0);
		else if (i * i == (unsigned int)nb)
			return (i);
		i++;
	}
	return (0);
}
