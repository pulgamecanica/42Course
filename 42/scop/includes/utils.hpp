#ifndef SCOP_UTILS_HPP
#define SCOP_UTILS_HPP

#include <sys/time.h>
#include <cstdint>
#include <vector>
#include <string>

namespace scop {

uint64_t          gettimeofday_ms();
uint64_t          timestamp_in_ms(uint64_t originTime);
std::vector<char> readFile(const std::string& filename);

} // namespace scop

#endif // SCOP_UTILS_HPP
