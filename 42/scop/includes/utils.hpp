#ifndef __UTILS_HPP__
# define __UTILS_HPP__
#include <sys/time.h>
#include <cstdint>
#include <vector>
#include <string>

uint64_t          timestamp_in_ms(uint64_t originTime);
uint64_t          gettimeofday_ms(void);
std::vector<char> readFile(const std::string& filename);
#endif