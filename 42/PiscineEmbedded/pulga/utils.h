#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdint.h>

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

static inline int	ft_strcmp(const char *s1, const char *s2)
{
  size_t	i;

  i = 0;
  while (s1[i] && s2[i] && s1[i] == s2[i])
    ++i;
  return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

static inline int	ft_strlen(const char *s) {
  int i = 0;
  while (s[i])
    i++;
  return (i);
}

static inline int	ft_strstr(const char *haystack, const char *needle) {
  int i, j;

  if (!*needle)
    return (1);
  for (i = 0; haystack[i]; i++) {
    for (j = 0; needle[j] && haystack[i + j] == needle[j]; j++) {}
      if (!needle[j])
        return (1);
  }
  return (0);
}

static inline uint8_t hex_char_to_val(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  return 255; // invalid
}

static inline uint8_t hex_pair_to_byte(char hi, char lo) {
  uint8_t high = hex_char_to_val(hi);
  uint8_t low  = hex_char_to_val(lo);
  if (high == 255 || low == 255)
    return 255;
  return (high << 4) | low;
}


#endif
