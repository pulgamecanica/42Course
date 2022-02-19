/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 00:19:39 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/16 00:19:49 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strstr(char *str, char *to_find)
{
	char	*ref;
	int		green_light;

	ref = to_find;
	green_light = 0;
	while (1)
	{
		if (*to_find == '\0' && (green_light || (&str[0] == str)))
			return (str - green_light);
		if (*to_find != *str)
		{
			green_light = 0;
			to_find = ref;
		}
		else
		{
			green_light++;
			to_find++;
		}
		if (*str == '\0')
			break ;
		str++;
	}
	return (0);
}
