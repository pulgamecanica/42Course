#ifndef __UTILS_HPP__
# define __UTILS_HPP__


#include <iostream>

#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "Config.hpp"
#include <webserv.hpp>

# define uint64_t unsigned long int

std::string &		strtrim(std::string & str, const char * separators = SEPARATORS);
uint64_t			timestamp_in_ms(void);
int 				ft_tolower(int);
int 				ft_toupper(int);
char * 				get_local_time(void);
const std::string &	readFileStream(std::ifstream const &, std::string & buff);
const std::string &	readFileString(std::string const &, std::string & buff);
bool 				isDirectory(const std::string & path);

#endif
