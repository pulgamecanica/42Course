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
    }
    {
        scop::point a{0.0f, 0};
        scop::point b{3.0f, 9.0f};
        scop::point c{16.0f, 4.0f};
        scop::Triangle t(a, b, c);

        std::cout << BLUE << t << ENDC << std::endl;
        for (int i = 10; i >= 0; --i) {
            for (int j = 0; j < 20; ++j) {
                scop::point p{(float)j, (float)i};
                std::cout << (t.isHovered(p) ? "." : " " ); 
            }
            std::cout << std::endl;
        }
    }

    /* Run the program until it's no longer required */
    pulga_scop->runScop();
    
    delete pulga_scop;
    return 0;
}
