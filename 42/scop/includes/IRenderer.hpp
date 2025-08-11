#ifndef SCOP_RENDERER_HPP
#define SCOP_RENDERER_HPP

#include "Math.hpp"

#include <cstdint>
#include <memory>

namespace scop {

class Window;
class Mesh;
class Texture;

enum class RenderMode : uint8_t { Faces, Textured };

struct CameraMatrices {
  Mat4 proj;
  Mat4 view;
};

struct FrameState {
  Mat4            model;        // object transform (rotation around centroid, translation, scaling)
  CameraMatrices  camera;       // perspective + view
  float           blendFactor;  // 0=faces, 1=texture (animate smoothly)
};

class IRenderer {
public:
  virtual ~IRenderer() = default;
  virtual void init(Window& window) = 0;
  virtual void resize(int width, int height) = 0;
  virtual void setMesh(const Mesh& mesh) = 0;
  virtual void setTexture(const Texture* texture) = 0; // nullptr => no texture bound
  virtual void setRenderMode(RenderMode mode) = 0;
  virtual void draw(const FrameState& state) = 0;
  virtual void waitIdle() = 0; // convenience for clean shutdown
};

} // namespace scop

#endif // SCOP_RENDERER_HPP
