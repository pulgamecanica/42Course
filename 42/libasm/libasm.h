#ifndef LIBASM_
#define LIBASM_

#ifdef __cplusplus
extern "C" size_t ft_strlen(const char* str);
extern "C" char *ft_strcpy(char *dst, const char *src);
extern "C" int ft_strcmp(const char *s1, const char *s2);
extern "C" ssize_t ft_write(int fd, const void *buf, size_t count);
#else
extern size_t ft_strlen(const char* str);
extern char *ft_strcpy(char *restrict dst, const char *restrict src);
extern int ft_strcmp(const char *s1, const char *s2);
extern ssize_t ft_write(int fd, const void *buf, size_t count);
#endif // __cplusplus


#endif // LIBASM_