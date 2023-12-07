//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __GUICONTAINER_HPP__
# define __GUICONTAINER_HPP__

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <iostream>
#include "Vulkan42.hpp"
/** This class is a generic Graphical User Interface Container
 * It allows you to load figures to the GUI Object
 * Figures are as follow:
 * Circle
 * Rectangle (Rounded corners, or not)

 * Figures:
 * BorderColor
 * BackgroundColor
 * BackgroundImage
 * BackgroundText
 * isHovered()
 * getX()
 * getY()
 * area()
 * perimeter()
 * draw()
 * 
 * Rectangle:
 * getWidth()
 * getHeight()
 * 
 * Circle:
 * getRad()
 * 
 * Triangle:
 * getHeight()
 * getAbsSideAB()
 * getAbsSideBC()
 * getAbsSideCA()
 * getSideA()
 * getSideB()
 * getSideC()
 */
class GUIContainer {
	public:
		GUIContainer(scop::Vulkan42 * & vk, scop::Window * & win);
		~GUIContainer();
		void	update();
		// virtual void drawContainer() const;
	private:
		scop::Vulkan42 * &	vk;
    scop::Window * & 		win;
		ImGui_ImplVulkanH_Window * wd;
		ImGuiIO io;
};

std::ostream&	operator<<(std::ostream&, const GUIContainer&);

#endif

