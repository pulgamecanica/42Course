/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 13:02:07 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/15 13:02:10 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strncat(char *dest, char *src, unsigned int nb)
{
	char	*temp;

	temp = dest;
	if (nb)
	{
		while (*dest != '\0')
			dest++;
		while (*src != 0)
		{
			if (nb <= 0)
				break ;
			else
				*dest++ = *src++;
			nb--;
		}
		*dest = '\0';
	}
	return (temp);
}
