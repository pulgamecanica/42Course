/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 23:10:36 by arosado-          #+#    #+#             */
/*   Updated: 2021/10/24 23:13:38 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdio.h>
# include <stdarg.h>
# include "../Libft/includes/libft.h"

void	ft_putnbrhex_fd(unsigned int nbr, int fd);
void	ft_putunsignednbr_fd(unsigned int nb, int fd);
void	ft_putnbrupperhex_fd(unsigned int nbr, int fd);
void	ft_recursiveaddr(unsigned long int nb, int is_recursive);
void	*ft_getf(char c);
int		count_nbr(int nbr, int fd);
int		count_chars(char c, int fd);
int		ft_printf(const char *str, ...);
int		count_conversions(char *str);
int		ft_putaddr_fd(void *ptr, int fd);
int		count_hexnum(unsigned int nbr, int fd);
int		count_unsigned(unsigned int nb, int fd);
int		ft_putstrnullcheck_fd(char *str, int fd);
int		count_hexnumupper(unsigned int nbr, int fd);
int		unsigned_num_counter(unsigned int nb, int base);

#endif
