//***************************//
//*Template by pulgamecanica*//
//***************************//
#ifndef __WINDOW_HPP__
# define __WINDOW_HPP__

# define GLFW_INCLUDE_VULKAN
# include <GLFW/glfw3.h>
# include <iostream>

namespace scop {
	class Window {
		public:
	    static unsigned int iconpng_error;
			Window(int width, int height);
			~Window();
			// Window(const Window&);
			int getWidth() const;
			int getHeight() const;
			// Window&	operator= (const Window&); // const for safety... not super nesessary
	    GLFWwindow* glfw_window;

	    class GLFWFailedException : public std::exception {
				public:
					virtual const char* what() const throw();
			};
	  	class WindowIconNotLoadedException : public std::exception {
				public:
					virtual const char* what() const throw();
			};
		private:
	    void resizeGLFWindow();

	    int width;
	    int height;
	    /**
	     * GLFWimage images[2];
	     * images[0] = load_icon("my_icon.png");
	     * images[1] = load_icon("my_icon_small.png");
	     * glfwSetWindowIcon(window, 2, images);
			 */
	};
	std::ostream&	operator<<(std::ostream&, const Window&);
}
#endif

