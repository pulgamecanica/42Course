/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 21:31:10 by arosado-          #+#    #+#             */
/*   Updated: 2021/09/28 21:31:12 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_READER_H
# define FILE_READER_H

# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdio.h>
# include <errno.h>
# include <libgen.h>
# include <string.h>

# define BUF_SIZE 4096
# define TOO_MANY_ARGUMENTS_ERROR "Too many arguments.\n"
# define NO_ARGUMENT_ERROR "File name missing.\n"

void	ft_putstr(int fd, char	*str);
void	display_content(char *file, int size, int ac, int current);
void	ft_tailheader(char *filename);
void	display_error(char *file);
void	tail_repeat(void);
void	ft_putchar(char c);
int		ft_file_size(char *filename);
int		ft_strcmp(char *s1, char *s2);
int		ft_atoi(char *str);

#endif
