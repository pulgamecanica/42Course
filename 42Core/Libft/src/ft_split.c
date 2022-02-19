/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 11:10:58 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/20 11:28:10 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	str_words(char const *str, char sep)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (str[i])
	{
		while (str[i] == sep && str[i])
			i++;
		if (str[i] == '\0')
			break ;
		while (str[i] != sep && str[i])
			i++;
		result++;
	}
	return (result);
}

static	char	*get_next_word(const char *str, int *i, char sep)
{
	int		j;
	char	*new;

	while (str[*i] == sep)
		if (str[(*i)++] == '\0')
			return (NULL);
	j = 0;
	while (str[*i + j] != sep && str[*i + j])
		j++;
	if (j != 0)
	{
		new = (char *)malloc(sizeof(char) * (j + 1));
		if (new == NULL)
			return (NULL);
		j = 0;
		while (str[*i] != sep && str[*i])
			new[j++] = str[(*i)++];
		new[j] = '\0';
	}
	else
		return (NULL);
	return (new);
}

char	**ft_split(char const *str, char sep)
{
	char	**list;
	char	*word;
	int		*i;
	int		j;
	int		k;

	list = (char **)malloc(sizeof(char *) * (str_words(str, sep) + 1));
	if (list == NULL)
		return (NULL);
	j = 0;
	i = &j;
	k = 0;
	while (k < str_words(str, sep))
	{
		word = get_next_word(str, i, sep);
		if (word == NULL)
			break ;
		list[k++] = word;
	}
	list[k] = NULL;
	return (list);
}
