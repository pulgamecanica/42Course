//***************************//
//*Template by pulgamecanica*//
//***************************//
#include "Scop.hpp"
#include "Window.hpp"
#include "utils.hpp"

using namespace scop;

static const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// Wrappers to handle event callbacks
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    static_cast<scop::Scop *>(glfwGetWindowUserPointer(window))->keyEvent(key, action, mods);
}
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    static_cast<scop::Scop *>(glfwGetWindowUserPointer(window))->mouseButtonEvent(button, action, mods);
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    static_cast<scop::Scop *>(glfwGetWindowUserPointer(window))->mouseMoveEvent(xpos, ypos);
}
static void window_size_callback(GLFWwindow* window, int width, int height) {
    static_cast<scop::Scop *>(glfwGetWindowUserPointer(window))->resize(width, height);
}

Scop::Scop() {
  status = ScopStatus::Menu;
  win = new Window(1000, 650);
  vulkan = new Vulkan42(win);
  gui = new GUIContainer(/*vulkan, */win);

  // NOTICE:  Change the conventional pointer to my custom pointer
  //          Requires a static_cast<Scop> in order to retreive it!
  glfwSetWindowUserPointer(win->glfw_window, this);

  // Seup the Wrappers which will triger scopObj.***Event()
  glfwSetKeyCallback(win->glfw_window, key_callback);
  glfwSetCursorPosCallback(win->glfw_window, cursor_position_callback);
  glfwSetMouseButtonCallback(win->glfw_window, mouse_button_callback);
  glfwSetWindowSizeCallback(win->glfw_window, window_size_callback);
}

Scop::~Scop() {
  delete gui;
  delete vulkan;
  delete win;
  std::cout << "Scop" << " destroyed" << std::endl;
}

void Scop::resize(int width, int height) {
  win->resizeWindow(width, height);
}

void Scop::mouseButtonEvent(int key, int action, int mods) {
  std::cout << "Mouse: " << GREEN << key << ENDC << " Action: " << GREEN << action << ENDC << " Mods: " << GREEN << mods << ENDC << std::endl;  

}
void Scop::mouseMoveEvent(double xpos, double ypos) {
  std::cout << "MouseMove: (" << xpos << "," << ypos << ")" << std::endl;
}

void Scop::keyEvent(int key, int action, int mods) {
  std::cout << "Key: " << GREEN << key << ENDC << " Action: " << GREEN << action << ENDC << " Mods: " << GREEN << mods << ENDC << std::endl;  
}

ScopStatus Scop::getStatus() const { return (status); };

bool Scop::checkStatus(ScopStatus status) const { return (this->status == status); }

void Scop::runScop() {
  created_at = gettimeofday_ms();
  updated_at = created_at;
  int fps = 0;
  while (!glfwWindowShouldClose(win->glfw_window)) {
    
      /* Render here */
      // glClear(GL_COLOR_BUFFER_BIT);

      /* Swap front and back buffers */
      // glfwSwapBuffers(win->glfw_window);

      /* Poll for and process events */
      glfwPollEvents();
      
      gui->update();

      if (timestamp_in_ms(updated_at) < 1000) {
          fps++;
          continue;
      }
      vulkan->drawFrame();
      // WAIT FOR FPS HERE:

      updated_at = gettimeofday_ms();
      std::cout << "[" << GREEN << updated_at - created_at << ENDC << "] " << fps << " fps " << YELLOW << (status == ScopStatus::Menu ? "Menu" : status == ScopStatus::Settings ? "Settings" : "Rendering") << ENDC << std::endl;
      fps = 0;
  }
}

std::ostream& scop::operator<<(std::ostream& s, const Scop& param) {
  // s << param.CONST_METHOD()
  (void)param;
  return (s);
}

