#ifndef UTILS_H
#define UTILS_H

static inline int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

static inline int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}

static inline int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}

static inline int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

static inline int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}

static inline int ft_isupper(int c)
{
  return (c >= 65 && c <= 90);
}

static inline int ft_islower(int c)
{
  return (c >= 97 && c <= 122);
}

static inline int	ft_tolower(int c)
{
	if (ft_isupper(c))
		return (c + 32);
	return (c);
}

static inline int	ft_toupper(int c)
{
	if (ft_islower(c))
		return (c - 32);
	return (c);
}

static inline int ft_toggle_case(int c)
{
  if (ft_islower(c))
    return ft_toupper(c);
  else if (ft_isupper(c))
    return ft_tolower(c);
  return (c);
}

#endif
