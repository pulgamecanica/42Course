#include "utils.hpp"
#include <fstream>
#include <stdexcept>

namespace scop {

uint64_t gettimeofday_ms() {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return (tv.tv_sec * static_cast<uint64_t>(1000)) + (tv.tv_usec / 1000);
}

uint64_t timestamp_in_ms(uint64_t originTime) {
  return gettimeofday_ms() - originTime;
}

std::vector<char> readFile(const std::string& filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);
  if (!file.is_open())
    throw std::runtime_error("Failed to open file: " + filename);

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);
  return buffer;
}

} // namespace scop
