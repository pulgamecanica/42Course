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

# define BUF_SIZE 4096
# define OPEN_FILE_ERROR "Cannot read file.\n"
# define CLOSE_FILE_ERROR OPEN_FILE_ERROR
# define READ_FILE_ERROR OPEN_FILE_ERROR
# define TOO_MANY_ARGUMENTS_ERROR "Too many arguments.\n"
# define NO_ARGUMENT_ERROR "File name missing.\n"

void	ft_putstr(char	*str);
void	display_content(char *file);

#endif
