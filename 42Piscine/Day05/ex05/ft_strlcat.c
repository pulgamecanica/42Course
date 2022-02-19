/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 13:02:22 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/15 13:02:25 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

unsigned int	count_string(char *str)
{
	unsigned int	count;

	count = 0;
	while (str[count] != '\0')
		count++;
	return (count);
}

unsigned int	ft_strlcat(char *dest, char *src, unsigned int size)
{
	unsigned int	temp;

	temp = count_string(dest) + count_string(src);
	if (count_string(dest) < size)
	{
		size = size - count_string(dest) - 1;
		dest = dest + count_string(dest);
		while (*src != 0)
		{
			if (size <= 0)
			{
				break ;
			}
			else
			{
				*dest++ = *src++;
			}
			size--;
		}
		*dest = '\0';
		return (temp);
	}
	else
		return (count_string(src) + size);
}
