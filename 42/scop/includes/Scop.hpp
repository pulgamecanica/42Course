#ifndef SCOP_HPP
#define SCOP_HPP

#ifndef SCOP_DEBUG
#define SCOP_DEBUG 1
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <cstdint>
#include <iostream>

#include "colors.hpp"
#include "Window.hpp"
#include "IRenderer.hpp"
#include "Vulkan42.hpp"
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

  Scop(const Scop&) = delete;
  Scop& operator=(const Scop&) = delete;

  ScopStatus getStatus() const;
  void       runScop();

  // GLFW event forwards
  void keyEvent(int key, int action, int mods);
  void mouseButtonEvent(int key, int action, int mods);
  void mouseMoveEvent(double xpos, double ypos);
  void mouseScrollEvent(double xoffset, double yoffset);
  void resize(int width, int height);

private:
  bool checkStatus(ScopStatus status) const;

  // App state
  ScopStatus                 status = ScopStatus::Menu;
  Window*                    win = nullptr;

  // Render
  std::unique_ptr<IRenderer> renderer;
  Camera                     camera;
  Mesh                       mesh;
  Texture                    texture;
  RenderMode                 mode = RenderMode::Faces;

  // Animation / transforms
  Vec3  translation{0.f, 0.f, 0.f};
  float autoRotateRadPerSec = 1.0f;
  float angle       = 0.f;
  float blendFactor = 0.f;
  float blendTarget = 0.f;

  // Timing
  uint64_t created_at = 0;
  uint64_t updated_at = 0;

  // Helpers
  void loadDefaultAssets();
  void updateBlend(float dt);
  Mat4 buildModel(float dt) const;
};

std::ostream& operator<<(std::ostream&, const Scop&);

} // namespace scop

#endif // SCOP_HPP
