/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 16:09:47 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/23 16:09:54 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1048
# endif

# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdio.h>
# include <errno.h>
# include <libgen.h>
# include <stdlib.h>

typedef struct s_read
{
	char	temp;
	int		r_total;
	int		r_count;
}		t_read;

void	ft_putstr(char *str);
char	*str_appendc(char *str, char c);
char	*get_next_line(int fd);
t_read	*new_read(void);

#endif
