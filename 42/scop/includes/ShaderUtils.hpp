#ifndef SCOP_SHADERUTILS_HPP
#define SCOP_SHADERUTILS_HPP

#include <string>
#include <vector>
#include "Math.hpp"

namespace scop {

// Define a CPU-side UBO layout that mirrors the shader
struct UboMVP {
  Mat4 proj;
  Mat4 view;
  Mat4 model;
  // If you want a separate normal matrix, add it here (as Mat4 or 3x3 padded)
};

struct PushConstants {
  float blendFactor; // for smooth faces<->texture transition
  // (padding handled in Vulkan pipeline definition if needed)
};

// Basic SPIR-V loader using your readFile()
std::vector<char> readSpirv(const std::string& path);

} // namespace scop

#endif // SCOP_SHADERUTILS_HPP
