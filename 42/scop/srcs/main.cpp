#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Scop.hpp"
#include "colors.hpp"

int main() {
  if (!glfwInit())
    return 1;

  try {
    scop::Scop app;
    app.runScop();
  } catch (const std::exception& e) {
    std::cerr << RED << "Fatal: " << e.what() << ENDC << std::endl;
    glfwTerminate();
    return 1;
  }

  return 0;
}
