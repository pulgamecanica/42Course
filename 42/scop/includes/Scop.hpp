#ifndef SCOP_HPP
#define SCOP_HPP

#ifndef SCOP_DEBUG
# define SCOP_DEBUG 1
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <cstdint>
#include <iostream>

#include "colors.hpp"
#include "Window.hpp"
#include "IRenderer.hpp"
#include "Vulkan42.hpp"   // IRenderer backend
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "ObjLoader.hpp"
#include "Math.hpp"
#include "utils.hpp"

namespace scop {

enum class ScopStatus : uint8_t {
  Menu,
  Settings,
  Rendering
};

class Scop {
public:
  Scop();
  ~Scop();

  ScopStatus  getStatus() const;
  void        runScop();

  // Event forwards from GLFW
  void        keyEvent(int key, int action, int mods);
  void        mouseButtonEvent(int key, int action, int mods);
  void        mouseMoveEvent(double xpos, double ypos);
  void        mouseScrollEvent(double xoffset, double yoffset);
  void        resize(int width, int height);

private:
  bool        checkStatus(ScopStatus status) const;

  // App state
  ScopStatus                      status = ScopStatus::Menu;
  Window*                         win = 0;

  // Render
  std::unique_ptr<IRenderer>      renderer;
  Camera                          camera;
  Mesh                            mesh;
  Texture                         texture;
  RenderMode                      mode = RenderMode::Faces;

  // Animation / transforms
  Vec3                            translation {0.f, 0.f, 0.f};
  float                           autoRotateRadPerSec = 1.0f; // ~57 deg/s
  float                           angle = 0.f;                 // accumulated
  float                           blendFactor = 0.f;           // 0..1
  float                           blendTarget = 0.f;           // matches mode

  // Timing
  uint64_t                        created_at = 0;
  uint64_t                        updated_at = 0;

  // Helpers
  void                            loadDefaultAssets(); // OBJ + texture (optional)
  void                            updateBlend(float dt);
  Mat4                            buildModel(float dt) const;  // rotate around centroid + translate
};

std::ostream& operator<<(std::ostream&, const Scop&);

} // namespace scop

#endif // SCOP_HPP
