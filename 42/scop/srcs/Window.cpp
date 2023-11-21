//***************************//
//*Template by pulgamecanica*//
//***************************//
#include "Window.hpp"
#include "lodepng.h"
#include "Scop.hpp"

using namespace scop;

unsigned int Window::iconpng_error = 0;

Window::Window(int width, int height) {
	this->width = width;
	this->height = height;
	this->glfw_window = glfwCreateWindow(width, height, "42 Pulga Scop", NULL, NULL);
  if (!this->glfw_window) {
      glfwTerminate();
      throw GLFWFailedException();
  } else {
  	glfwSetWindowAttrib(this->glfw_window, GLFW_RESIZABLE, GLFW_TRUE);
  	glfwMakeContextCurrent(this->glfw_window);
    glClearColor(0.42, 0.42, 0.42, 0);
  }
  /**
   * Decode pixels for GLFWimage using external library:
   * [ https://github.com/lvandeve/lodepng ]
	 */
	GLFWimage images[1];
	Window::iconpng_error = lodepng_decode32_file(&(images[0].pixels), (unsigned*)(&(images[0].width)), (unsigned*)(&(images[0].height)), "icons/42logo.png");
	if (iconpng_error) {
		glfwTerminate();
		throw WindowIconNotLoadedException();
	} else {
		glfwSetWindowIcon(this->glfw_window, 1, images);
		free(images[0].pixels);
	}
}

Window::~Window() {
  glfwDestroyWindow(glfw_window);
	glfwTerminate();
	std::cout << *this << " destroyed" << std::endl;
}

const char* Window::GLFWFailedException::what() const throw() {
	return ("GLFW function called failed");
}
const char* Window::WindowIconNotLoadedException::what() const throw() {
	return (lodepng_error_text(Window::iconpng_error));
}

void Window::resizeWindow(int width, int height) {
	std::cout << "new size: " << width << "x" << height << std::endl;
	if (width > 0)
		this->width = width;
	if (height > 0)
		this->height = height;
}

int Window::getWidth() const { return (width); }

int Window::getHeight() const { return (height); }

std::ostream& scop::operator<<(std::ostream& s, const Window& win) {
	s << GREEN << "ScopWindow " << win.getWidth() << "x" << win.getHeight() << ENDC; 
	return (s);
}

