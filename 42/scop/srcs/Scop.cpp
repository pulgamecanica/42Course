#include "Scop.hpp"
#include <algorithm>
#include <cmath>
#include <filesystem>

namespace scop {

namespace fs = std::filesystem;

// --- GLFW callback wrappers ---
static void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods) {
  (void)scancode;
  static_cast<Scop*>(glfwGetWindowUserPointer(w))->keyEvent(key, action, mods);
}
static void mouse_button_callback(GLFWwindow* w, int button, int action, int mods) {
  static_cast<Scop*>(glfwGetWindowUserPointer(w))->mouseButtonEvent(button, action, mods);
}
static void mouse_scroll_callback(GLFWwindow* w, double xoffset, double yoffset) {
  static_cast<Scop*>(glfwGetWindowUserPointer(w))->mouseScrollEvent(xoffset, yoffset);
}
static void cursor_position_callback(GLFWwindow* w, double xpos, double ypos) {
  static_cast<Scop*>(glfwGetWindowUserPointer(w))->mouseMoveEvent(xpos, ypos);
}
static void window_size_callback(GLFWwindow* w, int width, int height) {
  static_cast<Scop*>(glfwGetWindowUserPointer(w))->resize(width, height);
}

// --- Scop lifecycle ---
Scop::Scop(const std::string& initialModelPath) {
  win = new Window(1000, 650);

  renderer = std::unique_ptr<IRenderer>(new Vulkan42());
  renderer->init(*win);

  const float aspect = static_cast<float>(win->getWidth()) / static_cast<float>(win->getHeight());
  camera.setPerspective(60.0f * 3.1415926f / 180.f, aspect, 0.1f, 100.f);
  camera.setLookAt({3.f, 0.f, 3.0f}, {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f});

  // Discover models and pick the starting one. CLI flag wins; otherwise we
  // pick the first .obj alphabetically, falling back to the hard-coded teapot.
  scanModelsDirectory("assets/models");
  std::string startPath = initialModelPath;
  if (startPath.empty()) {
    startPath = modelPaths.empty() ? std::string("assets/models/teapot.obj")
                                   : modelPaths.front();
  }

  // Align currentModelIndex with the chosen path so 'O' cycles from there
  for (size_t i = 0; i < modelPaths.size(); ++i) {
    if (modelPaths[i] == startPath) { currentModelIndex = i; break; }
  }

  loadModel(startPath);
  loadDefaultTexture();
  renderer->setRenderMode(mode);

  status = ScopStatus::Rendering;

  glfwSetWindowUserPointer(win->glfw_window, this);
  glfwSetKeyCallback(win->glfw_window, key_callback);
  glfwSetCursorPosCallback(win->glfw_window, cursor_position_callback);
  glfwSetMouseButtonCallback(win->glfw_window, mouse_button_callback);
  glfwSetScrollCallback(win->glfw_window, mouse_scroll_callback);
  glfwSetWindowSizeCallback(win->glfw_window, window_size_callback);

  printControls();
}

Scop::~Scop() {
  if (renderer) renderer->waitIdle();
  delete win;
  std::cout << "Scop destroyed" << std::endl;
}

ScopStatus Scop::getStatus() const { return status; }
bool Scop::checkStatus(ScopStatus s) const { return status == s; }

void Scop::resize(int width, int height) {
  if (width <= 0 || height <= 0) return;
  win->resizeWindow(width, height);
  renderer->resize(width, height);
  camera.setAspect(static_cast<float>(width) / static_cast<float>(height));
}

// --- Input events ---
void Scop::mouseButtonEvent(int key, int action, int mods) {
  (void)mods;
  std::cout << "Mouse: " << GREEN << key << ENDC
            << " Action: " << GREEN << action << ENDC << std::endl;
}

void Scop::mouseMoveEvent(double xpos, double ypos) {
  (void)xpos; (void)ypos;
}

void Scop::mouseScrollEvent(double xoffset, double yoffset) {
  (void)xoffset; (void)yoffset;
}

void Scop::keyEvent(int key, int action, int mods) {
  (void)mods;
  // 'O' (model cycle) reacts on press only -- repeats would spam reloads
  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_O)      { cycleModel();        return; }
    if (key == GLFW_KEY_R)      { cycleRotationAxis(); return; }
    if (key == GLFW_KEY_ESCAPE) { glfwSetWindowShouldClose(win->glfw_window, GLFW_TRUE); return; }
  }
  if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

  constexpr float moveStep = 0.05f;
  switch (key) {
    case GLFW_KEY_A: translation.x -= moveStep; break;
    case GLFW_KEY_D: translation.x += moveStep; break;
    case GLFW_KEY_W: translation.y += moveStep; break;
    case GLFW_KEY_S: translation.y -= moveStep; break;
    case GLFW_KEY_Q: translation.z -= moveStep; break;
    case GLFW_KEY_E: translation.z += moveStep; break;
    case GLFW_KEY_T:
      if (mode == RenderMode::Faces) { mode = RenderMode::Textured; blendTarget = 1.f; }
      else                           { mode = RenderMode::Faces;    blendTarget = 0.f; }
      renderer->setRenderMode(mode);
      break;
    default: break;
  }
}

// --- Main loop ---
void Scop::runScop() {
  created_at = gettimeofday_ms();
  updated_at = created_at;
  int fps = 0;
  uint64_t fpsTimer = created_at;

  while (!glfwWindowShouldClose(win->glfw_window)) {
    const uint64_t now = gettimeofday_ms();
    const float dt = static_cast<float>(now - updated_at) / 1000.0f;
    updated_at = now;

    glfwPollEvents();

    updateBlend(dt);
    angle += autoRotateRadPerSec * dt;

    FrameState fs;
    fs.model       = buildModel(dt);
    fs.camera      = {camera.proj(), camera.view()};
    fs.blendFactor = blendFactor;

    renderer->draw(fs);

    ++fps;
    if (now - fpsTimer >= 1000) {
      std::cout << "[" << GREEN << (now - created_at) << ENDC << "] "
                << fps << " fps "
                << YELLOW << (status == ScopStatus::Menu ? "Menu"
                             : status == ScopStatus::Settings ? "Settings" : "Rendering")
                << ENDC << std::endl;
      fps = 0;
      fpsTimer = now;
    }
  }
}

// --- Helpers ---
void Scop::scanModelsDirectory(const std::string& dir) {
  modelPaths.clear();
  if (!fs::exists(dir) || !fs::is_directory(dir)) return;

  for (const auto& entry : fs::directory_iterator(dir)) {
    if (!entry.is_regular_file()) continue;
    if (entry.path().extension() == ".obj")
      modelPaths.push_back(entry.path().string());
  }
  std::sort(modelPaths.begin(), modelPaths.end());

  if (!modelPaths.empty()) {
    std::cout << GREEN << "[Scop] Found " << modelPaths.size()
              << " model(s) in " << dir << ENDC << std::endl;
  }
}

void Scop::loadModel(const std::string& path) {
  ObjLoader loader;
  try {
    mesh = loader.load(path, true, true);
    std::cout << GREEN << "[Scop] Model: " << path << ENDC << std::endl;
  } catch (const std::exception& e) {
    std::cout << RED << "OBJ load failed for '" << path << "': "
              << e.what() << ENDC << std::endl;
    mesh = Mesh({}, {}, {0, 0, 0}, {0, 1, 0});
  }
  // Reset angle so the new model starts in a clean orientation
  angle = 0.f;
  renderer->setMesh(mesh);
}

void Scop::cycleModel() {
  if (modelPaths.empty()) return;
  currentModelIndex = (currentModelIndex + 1) % modelPaths.size();
  loadModel(modelPaths[currentModelIndex]);
}

Vec3 Scop::axisFromIndex(int i) const {
  switch (i % 3) {
    case 0: return {1.f, 0.f, 0.f};
    case 1: return {0.f, 1.f, 0.f};
    default: return {0.f, 0.f, 1.f};
  }
}

void Scop::cycleRotationAxis() {
  rotationAxisIndex = (rotationAxisIndex + 1) % 3;
  static const char* names[] = {"X", "Y", "Z"};
  std::cout << GREEN << "[Scop] Rotation axis: " << names[rotationAxisIndex]
            << ENDC << std::endl;
}

void Scop::loadDefaultTexture() {
  // Same try-multiple-paths pattern as the model search
  const char* candidates[] = {
    "assets/textures/tex1.png",
    "assets/textures/ponies.png",
    "assets/textures/texture.png",
  };
  for (const char* path : candidates) {
    try {
      texture = Texture::fromPNG(path);
      renderer->setTexture(&texture);
      std::cout << GREEN << "[Scop] Texture: " << path << ENDC << std::endl;
      return;
    } catch (...) {}
  }
  std::cout << YELLOW << "[Scop] No texture found in assets/textures/, "
                         "using fallback white" << ENDC << std::endl;
  renderer->setTexture(nullptr);
}

void Scop::printControls() const {
  std::cout << "\n" << GREEN
            << "  Controls:\n"
            << "    W/A/S/D  : translate in X/Y\n"
            << "    Q/E      : translate in -Z/+Z\n"
            << "    T        : toggle texture (smooth blend)\n"
            << "    R        : cycle rotation axis (X -> Y -> Z)\n"
            << "    O        : cycle model from assets/models/\n"
            << "    ESC      : close window\n"
            << ENDC << std::endl;
}

void Scop::updateBlend(float dt) {
  constexpr float speed = 2.5f;
  const float delta = blendTarget - blendFactor;
  const float step = (delta > 0.f ? 1.f : -1.f) * speed * dt;
  if (std::fabs(delta) <= std::fabs(step))
    blendFactor = blendTarget;
  else
    blendFactor += step;
}

Mat4 Scop::buildModel(float /*dt*/) const {
  const Vec3 c  = mesh.centroid();
  const Vec3 ax = axisFromIndex(rotationAxisIndex);

  Mat4 toOrigin   = Mat4::translate({-c.x, -c.y, -c.z});
  Mat4 rotate     = Mat4::rotateAxisAngle(ax, angle);
  Mat4 fromOrigin = Mat4::translate({c.x, c.y, c.z});
  Mat4 world      = Mat4::translate(translation);

  return world * (fromOrigin * (rotate * toOrigin));
}

std::ostream& operator<<(std::ostream& s, const Scop& param) {
  (void)param;
  return s;
}

} // namespace scop
