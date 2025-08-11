#include "Scop.hpp"
#include <cmath>

using namespace scop;

// --- GLFW callback wrappers (unchanged) ---
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

// --- Scop ---
Scop::Scop() {
  win = new Window(1000, 650);

  // Renderer
  renderer = std::unique_ptr<IRenderer>(new Vulkan42());
  renderer->init(*win);

  // Camera: perspective required by subject
  const float aspect = float(win->getWidth()) / float(win->getHeight());
  camera.setPerspective( /*fov*/ 60.0f * 3.1415926f/180.f, aspect, /*znear*/ 0.1f, /*zfar*/ 100.f );
  camera.setLookAt( /*eye*/ {0.f, 0.f, 3.0f}, /*center*/ {0.f,0.f,0.f}, /*up*/ {0.f,1.f,0.f} );

  // Assets
  loadDefaultAssets();
  renderer->setMesh(mesh);
  renderer->setTexture(nullptr); // start in Faces mode
  renderer->setRenderMode(mode);

  status = ScopStatus::Rendering;

  // User pointer + callbacks
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
  camera.setAspect(float(width) / float(height));
}

void Scop::mouseButtonEvent(int key, int action, int mods) {
  (void)mods;
  std::cout << "Mouse: " << GREEN << key << ENDC
            << " Action: " << GREEN << action << ENDC << std::endl;
}
void Scop::mouseMoveEvent(double xpos, double ypos) {
  std::cout << "MouseMove: (" << xpos << "," << ypos << ")\n";
}
void Scop::mouseScrollEvent(double xoffset, double yoffset) {
  std::cout << "MouseScroll: (" << xoffset << "," << yoffset << ")\n";
  // (Optional) zoom by scrolling: move eye along view dir and rebuild lookAt
}
void Scop::keyEvent(int key, int action, int mods) {
  std::cout << "Key: " << GREEN << key << ENDC << " Action: " << GREEN << action << ENDC << " Mods: " << GREEN << mods << ENDC << std::endl;  
  (void)mods;
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    // Move along axes: A/D -> X-,X+, W/S -> Y+,Y-, Q/E -> Z-,Z+
    switch (key) {
      case GLFW_KEY_A: translation.x -= 0.05f; break;
      case GLFW_KEY_D: translation.x += 0.05f; break;
      case GLFW_KEY_W: translation.y += 0.05f; break;
      case GLFW_KEY_S: translation.y -= 0.05f; break;
      case GLFW_KEY_Q: translation.z -= 0.05f; break;
      case GLFW_KEY_E: translation.z += 0.05f; break;

      // Toggle render mode (Faces/Textured) with smooth transition
      case GLFW_KEY_T:
        if (mode == RenderMode::Faces) { mode = RenderMode::Textured; blendTarget = 1.f; }
        else                           { mode = RenderMode::Faces;   blendTarget = 0.f; }
        renderer->setRenderMode(mode);
        break;

      default: break;
    }
  }
}

void Scop::runScop() {
  created_at = gettimeofday_ms();
  updated_at = created_at;
  int fps = 0;

  while (!glfwWindowShouldClose(win->glfw_window)) {
    const uint64_t now = gettimeofday_ms();
    const float dt = (now - updated_at) / 1000.0f;
    updated_at = now;

    glfwPollEvents();

    // Animate blend + auto-rotation
    updateBlend(dt);
    angle += autoRotateRadPerSec * dt;

    // Build per-frame state
    FrameState fs;
    fs.model = buildModel(dt);
    fs.camera = { camera.proj(), camera.view() };
    fs.blendFactor = blendFactor;

    renderer->draw(fs);

    // Simple FPS print each second (optional)
    ++fps;
    static uint64_t t0 = now;
    if (now - t0 >= 1000) {
      std::cout << "[" << GREEN << (now - created_at) << ENDC << "] "
                << fps << " fps "
                << YELLOW << (status == ScopStatus::Menu ? "Menu"
                             : status == ScopStatus::Settings ? "Settings" : "Rendering")
                << ENDC << std::endl;
      fps = 0;
      t0 = now;
    }
  }
}

// --- Helpers ---
void Scop::loadDefaultAssets() {
  // OBJ
  ObjLoader loader;
  try {
    mesh = loader.load("models/42logo.obj", /*computeNormals*/true, /*triangulate*/true);
  } catch (const std::exception& e) {
    std::cout << RED << "OBJ load failed: " << e.what() << ENDC << std::endl;
    // Fallback: empty mesh; renderer should handle gracefully
    mesh = Mesh(std::vector<Vertex>{}, std::vector<uint32_t>{}, {0,0,0}, {0,1,0});
  }

  // Optional texture: if present you can enable with 'T'
  try {
    texture = Texture::fromPNG("textures/42logo.png");
    // Don’t bind yet; Faces mode is default (blendTarget stays 0).
    renderer->setTexture(&texture);
  } catch (...) {
    // Texture optional; keep renderer texture as nullptr if loading fails
    renderer->setTexture(nullptr);
  }
}

void Scop::updateBlend(float dt) {
  const float speed = 2.5f; // seconds to fully switch ~0.4s
  const float delta = blendTarget - blendFactor;
  const float step = (delta > 0.f ? 1.f : -1.f) * speed * dt;
  if (std::fabs(delta) <= std::fabs(step)) blendFactor = blendTarget;
  else blendFactor += step;
}

Mat4 Scop::buildModel(float /*dt*/) const {
  // Rotate around mesh centroid along its main axis, then apply translation
  const Vec3 c = mesh.centroid();
  const Vec3 ax = mesh.mainAxis();

  Mat4 T_to = Mat4::translate({ c.x, c.y, c.z });
  Mat4 T_back = Mat4::translate({ -c.x, -c.y, -c.z });
  Mat4 R = Mat4::rotateAxisAngle(ax, angle);
  Mat4 T_world = Mat4::translate(translation);

  // Order depends on your math convention (document it!):
  // Here: column-major with column vectors → M = T_world * T_to * R * T_back
  return T_world * (T_to * (R * T_back));
}

std::ostream& scop::operator<<(std::ostream& s, const Scop& param) {
  (void)param; return s;
}
