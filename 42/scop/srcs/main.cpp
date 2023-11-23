    //***************************//
//*Template by pulgamecanica*//
//***************************//
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// #include "Window.hpp"
#include "Scop.hpp"

#include "Figure.hpp"
#include "Triangle.hpp"

int main(void) {
    scop::Scop * pulga_scop;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Initialize the program */
    try {
        pulga_scop = new scop::Scop();
    } catch (const scop::Window::GLFWFailedException& e) {
        std::cout << "GLFW Exception: " << RED << e.what() << ENDC << std::endl;
        return -1;
    } catch (const scop::Window::WindowIconNotLoadedException& e) {
        std::cout << "Window Icon Exception: " << RED << e.what() << ENDC << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cout << "Error: " << RED << e.what() << ENDC << std::endl;
    }
    {
        scop::point a{0.0f, 0};
        scop::point b{30.0f, 4.0f};
        scop::point c{42.0f, 22.0f};
        scop::Triangle t(a, b, c);

        std::cout << BLUE << t << ENDC << std::endl;
        // t.draw();
    }

    /* Run the program until it's no longer required */
    pulga_scop->runScop();
    
    delete pulga_scop;
    return 0;
}
