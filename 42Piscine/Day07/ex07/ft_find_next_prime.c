/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_next_prime.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 01:31:08 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/18 01:31:09 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
int	ft_sqrt(int nb)
{
	unsigned int	i;

	if (nb < 0)
		return (0);
	if (nb < 2)
		return (1);
	i = 0;
	if (nb == 2147483647)
		return (1);
	while (1)
	{
		if (i * i >= (unsigned int)nb)
			return (i);
		i++;
	}
	return (i);
}

int	ft_is_prime(int nb)
{
	int	index;

	if (nb % 2 == 0)
		return (0);
	index = 3;
	while (index < ft_sqrt(nb))
	{
		if (nb % index == 0)
			return (0);
		index = index + 2;
	}
	return (1);
}

int	ft_find_next_prime(int nb)
{
	if (nb <= 2)
		return (2);
	if (ft_is_prime(nb))
		return (nb);
	if (nb % 2 == 0)
		nb++;
	while (!ft_is_prime(nb))
		nb = nb + 2;
	return (nb);
}
