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
		GUIContainer(/*scop::Vulkan42 * & vk, */scop::Window * & win);
		~GUIContainer();
		void	update();
		// virtual void drawContainer() const;
	private:
		void SetupVulkan(ImVector<const char*> extensions);
		void SetupVulkanWindow(ImGui_ImplVulkanH_Window * wd, VkSurfaceKHR surface, int w, int h);
		void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
		void FramePresent(ImGui_ImplVulkanH_Window* wd);
		bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
		VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
		

		// scop::Vulkan42 * &					vk;
		scop::Window * & 						win;
		ImGui_ImplVulkanH_Window * 	wd;
    ImVec4 clear_color;
		ImGuiIO io;

  	VkSurfaceKHR							surface;
  	GLFWwindow * & 						window;
		ImGui_ImplVulkanH_Window	g_MainWindowData;
		VkInstance               	g_Instance;
		VkPhysicalDevice         	g_PhysicalDevice;
		VkDevice                 	g_Device;
		uint32_t                 	g_QueueFamily;
		VkQueue                  	g_Queue;
		VkDebugReportCallbackEXT 	g_DebugReport;
		VkPipelineCache          	g_PipelineCache;
		VkDescriptorPool         	g_DescriptorPool;
		int                     	g_MinImageCount;
		bool                     	g_SwapChainRebuild;
		bool show_demo_window;
  	bool show_another_window;
};

std::ostream&	operator<<(std::ostream&, const GUIContainer&);

#endif

