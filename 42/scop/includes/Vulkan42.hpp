//***************************//
//*Template by pulgamecanica*//
//***************************//
#ifndef __VULKAN42_HPP__
# define __VULKAN42_HPP__

#ifndef SCOP_DEBUG
# define SCOP_DEBUG 1
#endif

# ifndef GLFW_INCLUDE_VULKAN
# define GLFW_INCLUDE_VULKAN
# endif
#include <GLFW/glfw3.h>
#include "colors.hpp"
#include "Window.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <optional>


namespace scop {
	struct QueueFamInd {
    // C++17 feature
    // std::optional is a wrapper that contains no value until you assign something to it.
    // At any point you can query if it contains a value or not by calling its has_value()
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {      
      return graphicsFamily.has_value() && presentFamily.has_value();
    }
  };
	class Vulkan42 {
		public:
			Vulkan42(const Window & win);
			~Vulkan42();
		private:
			void 							createVkInstance(const char * application_name);
			bool							isPhysicalDeviceSuitable(VkPhysicalDevice device);
			void							pickFirstSuitablePhysicalDevice();
			QueueFamInd				findQueueFamilies(VkPhysicalDevice device);
			void							createLogicalDevice();
			void							createSurface();
			bool				 			checkDeviceExtensionSupport(VkPhysicalDevice device);
		
			VkInstance	  		instance;
      VkSurfaceKHR      surface;
			/* 
       * A physical device in this case could be a Graphics Card for example
       * In my case, the device selected is: AMD Radeon Vega 3 Graphics (RADV RAVEN2)
       */
			VkPhysicalDevice	physicalDevice;
      VkDevice          device;
			VkQueue           graphicsQueue;
      VkQueue           presentQueue;
			const Window	&		win;
	};
	std::ostream&	operator<<(std::ostream&, const Vulkan42&);
}
#endif

