/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 19:06:29 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/27 19:06:32 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*g_new_str;
char	*g_end;
int		g_size;
char	**g_a;
char	*g_helper;
int		g_n_words = 0;

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

int	ft_strlen(char *str)
{
	int	counter;

	counter = 0;
	while (str[counter] != '\0')
		counter++;
	return (counter);
}

char	*get_word(char *str, char *charset)
{
	if (ft_strstr(str, charset))
	{
		str += ft_strlen(charset);
		if (ft_strstr(str, charset))
		{
			g_end = ft_strstr(str, charset);
			g_size = g_end - str;
			if (g_size > 0)
			{
				g_new_str = (char *)malloc(sizeof(char) * (g_size + 1));
				if (g_new_str == NULL)
					return (0);
				g_size = 0;
				while (str != g_end)
				{
					g_new_str[g_size] = *str;
					g_size++;
					str++;
				}
				g_new_str[g_size] = '\0';
				return (g_new_str);
			}
		}
	}
	return (0);
}

char	**ft_split(char *s, char *cs)
{
	g_helper = s;
	while (ft_strstr(g_helper, cs))
	{
		g_helper = ft_strstr(g_helper, cs);
		g_helper += ft_strlen(cs);
		if (get_word(g_helper, cs) != (g_helper + 1))
			g_n_words++;
	}
	g_a = (char **)malloc(sizeof(char *) * (g_n_words + 1));
	if (g_a == NULL)
		return (0);
	while (get_word(s, cs) != 0)
	{
		if (get_word(s, cs))
		{
			*g_a = (char *)malloc(sizeof(char) * (ft_strlen(get_word(s, cs))));
			*g_a = get_word(s, cs);
			g_a++;
		}
		s += ft_strlen(get_word(s, cs)) + ft_strlen(cs);
	}
	*g_a = 0;
	return (g_a - (g_n_words - 1));
}
