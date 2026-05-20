#include "Scop.hpp"
#include <cmath>

namespace scop {

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
Scop::Scop() {
  win = new Window(1000, 650);

  renderer = std::unique_ptr<IRenderer>(new Vulkan42());
  renderer->init(*win);

  const float aspect = static_cast<float>(win->getWidth()) / static_cast<float>(win->getHeight());
  camera.setPerspective(60.0f * 3.1415926f / 180.f, aspect, 0.1f, 100.f);
  camera.setLookAt({3.f, 0.f, 3.0f}, {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f});

  loadDefaultAssets();
  renderer->setMesh(mesh);
  renderer->setTexture(nullptr);
  renderer->setRenderMode(mode);

  status = ScopStatus::Rendering;

  glfwSetWindowUserPointer(win->glfw_window, this);
  glfwSetKeyCallback(win->glfw_window, key_callback);
  glfwSetCursorPosCallback(win->glfw_window, cursor_position_callback);
  glfwSetMouseButtonCallback(win->glfw_window, mouse_button_callback);
  glfwSetScrollCallback(win->glfw_window, mouse_scroll_callback);
  glfwSetWindowSizeCallback(win->glfw_window, window_size_callback);
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
void Scop::loadDefaultAssets() {
  ObjLoader loader;
  try {
    mesh = loader.load("assets/models/42.obj", true, true);
  } catch (const std::exception& e) {
    std::cout << RED << "OBJ load failed: " << e.what() << ENDC << std::endl;
    mesh = Mesh({}, {}, {0, 0, 0}, {0, 1, 0});
  }

  try {
    texture = Texture::fromPNG("assets/textures/tex1.png");
    renderer->setTexture(&texture);
  } catch (...) {
    std::cout << "WOOO\n\n\nCouldnt convert!\n\n\n" ;
    renderer->setTexture(nullptr);
  }
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
  const Vec3 ax = mesh.mainAxis();

  Mat4 toOrigin  = Mat4::translate({-c.x, -c.y, -c.z});
  Mat4 rotate    = Mat4::rotateAxisAngle(ax, angle);
  Mat4 fromOrigin = Mat4::translate({c.x, c.y, c.z});
  Mat4 world     = Mat4::translate(translation);

  return world * (fromOrigin * (rotate * toOrigin));
}

std::ostream& operator<<(std::ostream& s, const Scop& param) {
  (void)param;
  return s;
}

} // namespace scop
