/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copiedft_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 11:10:37 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/18 16:35:28 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	nwords(char *s, char c)
{
	int	a;
	int	is_word;

	a = 0;
	is_word = 0;
	while (*s)
	{
		if (is_word == 0 && *s != c)
		{
			is_word = 1;
			a++;
		}
		else if (is_word == 1 && *s == c)
			is_word = 0;
		s++;
	}
	return (a);
}

static int	lenword(char *s, char c)
{
	int	len;

	len = 0;
	while (*s != c && *s != '\0')
	{
		len++;
		s++;
	}
	return (len);
}

static void	*freetab(char **tab, int i)
{
	while (i-- >= 0)
		free(tab[i]);
	free(tab);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		words;
	char	**tab;
	int		i;

	if (!s)
		return (NULL);
	words = nwords((char *)s, c);
	tab = (char **)ft_calloc(sizeof(char *), words + 1);
	if (!tab)
		return (NULL);
	i = 0;
	while (words--)
	{
		while (*s == c && *s != '\0')
			s++;
		tab[i] = ft_substr((char *)s, 0, lenword((char *)s, c));
		if (!tab[i])
			return (freetab(tab, i));
		s = s + lenword((char *)s, c);
		i++;
	}
	tab[i] = NULL;
	return (tab);
}
