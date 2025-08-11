#include "ShaderUtils.hpp"
#include "utils.hpp"

namespace scop {

std::vector<char> readSpirv(const std::string& path) {
  return readFile(path);
}

} // namespace scop
