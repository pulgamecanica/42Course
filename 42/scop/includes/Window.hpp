#ifndef SCOP_WINDOW_HPP
#define SCOP_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>

namespace scop {

class Window {
public:
  static unsigned int iconpng_error;

  Window(int width, int height);
  ~Window();

  int          getWidth() const;
  int          getHeight() const;
  void         resizeWindow(int width, int height);

  GLFWwindow*  glfw_window;

  class GLFWFailedException : public std::exception {
  public:
    const char* what() const noexcept override;
  };

  class WindowIconNotLoadedException : public std::exception {
  public:
    const char* what() const noexcept override;
  };

private:
  int _width;
  int _height;
};

std::ostream& operator<<(std::ostream&, const Window&);

} // namespace scop

#endif // SCOP_WINDOW_HPP

