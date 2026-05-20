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
#include <string>
#include <vector>

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
  // initialModelPath empty => use the first .obj found in assets/models
  explicit Scop(const std::string& initialModelPath = "");
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

  // Available models (discovered from assets/models at startup)
  std::vector<std::string> modelPaths;
  size_t                   currentModelIndex = 0;

  // Animation / transforms
  Vec3  translation{0.f, 0.f, 0.f};
  float autoRotateRadPerSec = 1.0f;
  float angle       = 0.f;
  float blendFactor = 0.f;
  float blendTarget = 0.f;

  // Rotation axis: 0=X, 1=Y, 2=Z. Default to X per project preference.
  int  rotationAxisIndex = 0;
  Vec3 axisFromIndex(int i) const;

  // Timing
  uint64_t created_at = 0;
  uint64_t updated_at = 0;

  // Helpers
  void scanModelsDirectory(const std::string& dir);
  void loadModel(const std::string& path);
  void cycleModel();
  void cycleRotationAxis();
  void loadDefaultTexture();
  void printControls() const;
  void updateBlend(float dt);
  Mat4 buildModel(float dt) const;
};

std::ostream& operator<<(std::ostream&, const Scop&);

} // namespace scop

#endif // SCOP_HPP
