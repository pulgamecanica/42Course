#include "Window.hpp"
#include "lodepng.h"
#include "colors.hpp"

namespace scop {

unsigned int Window::iconpng_error = 0;

Window::Window(int width, int height) : _width(width), _height(height) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfw_window = glfwCreateWindow(_width, _height, "42 Pulga Scop", nullptr, nullptr);
  if (!glfw_window) {
    glfwTerminate();
    throw GLFWFailedException();
  }
  glfwSetWindowAttrib(glfw_window, GLFW_RESIZABLE, GLFW_TRUE);

  GLFWimage images[1];
  iconpng_error = lodepng_decode32_file(
    &images[0].pixels,
    reinterpret_cast<unsigned*>(&images[0].width),
    reinterpret_cast<unsigned*>(&images[0].height),
    "icons/42logo.png"
  );
  if (iconpng_error) {
    glfwTerminate();
    throw WindowIconNotLoadedException();
  }
  glfwSetWindowIcon(glfw_window, 1, images);
  free(images[0].pixels);
}

Window::~Window() {
  glfwDestroyWindow(glfw_window);
  glfwTerminate();
  std::cout << *this << " destroyed" << std::endl;
}

const char* Window::GLFWFailedException::what() const noexcept {
  return "GLFW window creation failed";
}

const char* Window::WindowIconNotLoadedException::what() const noexcept {
  return lodepng_error_text(Window::iconpng_error);
}

void Window::resizeWindow(int width, int height) {
  if (width > 0)  _width = width;
  if (height > 0) _height = height;
}

int Window::getWidth() const  { return _width; }
int Window::getHeight() const { return _height; }

std::ostream& operator<<(std::ostream& s, const Window& win) {
  s << GREEN << "ScopWindow " << win.getWidth() << "x" << win.getHeight() << ENDC;
  return s;
}

} // namespace scop
