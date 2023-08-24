/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 22:48:02 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/20 22:48:03 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strjoin(char *buf, char *add) {
	char	*newbuf;
	int		len;

	if (buf == NULL)
		len = 0;
	else
		len = ft_strlen(buf);
	newbuf = ft_calloc(sizeof(*newbuf), (len + ft_strlen(add) + 1));
	if (newbuf == NULL)
		return (NULL);
	if (buf != NULL)
		ft_strlcat(newbuf, buf, len + 1);
	free(buf);
	ft_strlcat(newbuf, add, len + ft_strlen(add) + 1);
	return (newbuf);
}

/*

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new;
	int		i;
	int		k;

	if (s1 == NULL || s1 == NULL)
		return (NULL);
	new = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (new == NULL)
		return (NULL);
	i = -1;
	while (s1[++i] != '\0')
		new[i] = s1[i];
	k = -1;
	while (s2[++k] != '\0')
		new[i + k] = s2[k];
	new[i + k] = '\0';
	return (new);
}
*/
