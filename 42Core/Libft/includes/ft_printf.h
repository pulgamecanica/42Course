/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 16:17:35 by arosado-          #+#    #+#             */
/*   Updated: 2021/11/03 21:10:35 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "libft.h"
# include <limits.h>

typedef struct s_numformat
{
	char	*prefix;
	char	*prezeros;
	char	*prewidth;
}		t_numf;

typedef struct s_flags
{
	unsigned int	flag_sp;
	unsigned int	flag_minus;
	unsigned int	flag_zero;
	unsigned int	flag_plus;
	unsigned int	flag_dot;
	unsigned int	flag_cardinal;
	unsigned int	minfw;
	unsigned int	maxfw;
}	t_flags;

int		ft_printf(const char *str, ...);
int		ft_fchar(char c, t_flags *flags);
int		ft_fstr(char *str, t_flags *flags);
int		ft_fnum(int num, t_flags *flags);
int		ft_funsnum(unsigned int num, t_flags *flags);
int		ft_fptr(unsigned long int nbr, t_flags *flags);
int		ft_fhex(unsigned int n, t_flags *flags);
int		ft_fhexupper(unsigned int n, t_flags *flags);
int		ft_numlen(unsigned long int nbr, int baselen, int f);
void	set_prefix(t_numf *n, char *str);
void	set_prezeros(t_numf *n, char *str);
void	set_prewidth(t_numf *n, char *str);
void	free_format(t_numf *format);
void	ft_putanynbr_fd(unsigned long int nbr, const char *base, int f);
char	*ft_getprefix(char c, int times);
char	*ft_strfreedup(char *src, char *to_free);
char	*ft_strfreesub(char *str, int end, char *to_free);
void	*ft_f(char c);
t_numf	*clean_format(void);
t_flags	*getfl(const char *str, int *i);

#endif
