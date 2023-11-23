//***************************//
//*Template by pulgamecanica*//
//***************************//
#include "Vulkan42.hpp"

using namespace scop;

static const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

Vulkan42::Vulkan42(const Window & win): win(win) {
	physicalDevice = VK_NULL_HANDLE;
  device         = VK_NULL_HANDLE;

  createVkInstance("Scop");
  createSurface();
  pickFirstSuitablePhysicalDevice();
  createLogicalDevice();
  createSwapChain();
  createImageViews();
}

void  Vulkan42::createVkInstance(const char * application_name) {
  /* VkApplicationInfo */
  VkApplicationInfo appInfo{};

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // ? -> What is sType?
  appInfo.pApplicationName = application_name;
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  /* Extensions */
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  
  /* VkInstanceCreateInfo */
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = glfwExtensionCount; // ? -> What is Extensions? What is an extension?
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount = 0; // What is this line doing?

  /* Finally VkInstance! */
  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }

  /* Debug */
  if (SCOP_DEBUG) {
	  std::cout << "Initializing Vulkan Instance <"<< BLUE << application_name << ENDC << ">" << std::endl;
	  std::cout << " * " << GREEN << "ApplicationInfo" << ENDC << ":\t\t" << "version(" << BLUE << VK_MAKE_VERSION(1, 0, 0) << ENDC << ")" << std::endl;
	  std::cout << " * " << GREEN << "ApplicationInfo" << ENDC << ":\t\t" << "engine(" << BLUE << appInfo.pEngineName << " - version " << VK_MAKE_VERSION(1, 0, 0) << ENDC << ")" << std::endl;
	  std::cout << " * " << GREEN << "ApplicationInfo" << ENDC << ":\t\t" << "apiVersion("<< BLUE  << VK_API_VERSION_1_0 << ENDC << ")" << std::endl;
	  std::cout << " * " << GREEN << "VkInstanceCreateInfo" << ENDC << ":\t" << "Enabled Extension Count(" << BLUE << glfwExtensionCount << ENDC << ")" << std::endl;
	  std::cout << " * " << GREEN << "VkInstanceCreateInfo" << ENDC << ":\t" << "Extension names:" << std::endl;
	  for (uint32_t i = 0; i < glfwExtensionCount; ++i)
	  	std::cout << " *\t\t\t\t - " << BLUE << glfwExtensions[i] << ENDC << std::endl;
	  std::cout << " * " << GREEN << "VkInstanceCreateInfo" << ENDC << ":\t" << "Enabled Layer Count(" << BLUE << createInfo.enabledLayerCount << ENDC << ")" << std::endl << std::endl;
	}
}

void Vulkan42::createSurface() {
  if (glfwCreateWindowSurface(instance, win.glfw_window, nullptr, &surface) != VK_SUCCESS) {
      throw std::runtime_error("failed to create window surface!");
  }
}

bool Vulkan42::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const VkExtensionProperties& extension : availableExtensions)
        requiredExtensions.erase(extension.extensionName);
    return requiredExtensions.empty();
}

bool Vulkan42::isPhysicalDeviceSuitable(VkPhysicalDevice device) {
  QueueFamInd indices = findQueueFamilies(device);

  bool extensionsSupported = checkDeviceExtensionSupport(device);

  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;

  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
  if (SCOP_DEBUG) {
  	std::cout << "\tName: " << BLUE << deviceProperties.deviceName << ENDC;
	  std::cout << "\tType: " << BLUE << (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER ? "Other" 
	    : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "Integrated with Host"
	    : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "Separated connected to Host" 
	    : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU ? "Virtual (Node || Environment)" 
	    : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU ? "Running on the same processor" : "") << ENDC << std::endl << std::endl;
  }
  bool swapChainAdequate = false;
	if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, false);
    swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
  //	Features and Properties that will be needed
  //	Known Phisical Device Type
  //	Geometry Shader
  //  Support Swap Chain extension (some devices don't even handle GUI and wont have this)
  //  Swap Chain supports at least one image format and one presentation mode
  return deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_OTHER &&
         deviceFeatures.geometryShader &&
         indices.isComplete() &&
         extensionsSupported &&
         swapChainAdequate;
}

void  Vulkan42::pickFirstSuitablePhysicalDevice() {
  uint32_t deviceCount = 0;

  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }
  
  std::vector<VkPhysicalDevice> devices(deviceCount);

  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
	std::cout << "Analysing Devices ..." << std::endl;
  for (const VkPhysicalDevice& device : devices) {
    if (isPhysicalDeviceSuitable(device)) {
      physicalDevice = device;
      break;
    }
  }
  if (physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }
}

QueueFamInd Vulkan42::findQueueFamilies(VkPhysicalDevice device) {
  QueueFamInd indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const VkQueueFamilyProperties& queueFamily : queueFamilies) {
      if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
          indices.graphicsFamily = i;
      }
      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
      if (presentSupport) {
        indices.presentFamily = i;
      }
      if (indices.isComplete()) {
          break;
      }
      i++;
  }
  return indices;
}

void Vulkan42::createLogicalDevice() {
  QueueFamInd indices = findQueueFamilies(physicalDevice);
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  // Set to exclude same indices (sets => unique keys)
  std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

  float queuePriority = 1.0f;
  if (SCOP_DEBUG)
  	std::cout << "Queue Families Infos ..." << std::endl;
  for (uint32_t queueFamily : uniqueQueueFamilies) {
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamily;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back(queueCreateInfo);
      if (SCOP_DEBUG)
      	std::cout <<  "\t" << "Index(" << BLUE << queueFamily << ENDC << ")" << std::endl << std::endl;
  }

  VkPhysicalDeviceFeatures deviceFeatures{};
  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions.data();
  createInfo.enabledLayerCount = 0;
  if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
      throw std::runtime_error("failed to create logical device!");
  }
  vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
  vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void Vulkan42::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, true);
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    swapChainExtent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
	    imageCount = swapChainSupport.capabilities.maxImageCount;
		}
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = swapChainExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


		QueueFamInd indices = findQueueFamilies(physicalDevice);
		uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

		if (indices.graphicsFamily != indices.presentFamily) {
		    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		    createInfo.queueFamilyIndexCount = 2;
		    createInfo.pQueueFamilyIndices = queueFamilyIndices;
		} else {
		    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		    createInfo.queueFamilyIndexCount = 0; // Optional
		    createInfo.pQueueFamilyIndices = nullptr; // Optional
		}
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;
		/* Finally create the SwapChain */
		if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
  	  throw std::runtime_error("failed to create swap chain!");
		}
		/* Also asign the images of the swap chain to our member vector */
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
		swapChainImageFormat = surfaceFormat.format;
}

void Vulkan42::createImageViews() {
	swapChainImageViews.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); ++i) {
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
    	throw std::runtime_error("failed to create image views!");
		}
	}
}

SwapChainSupportDetails Vulkan42::querySwapChainSupport(VkPhysicalDevice device, bool debug) {
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
		if (formatCount != 0) {
		    details.formats.resize(formatCount);
		    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
		if (presentModeCount != 0) {
		    details.presentModes.resize(presentModeCount);
		    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}
		if (SCOP_DEBUG && debug) {
			std::cout << "Swap Chain Info:" << std::endl;
			std::cout << "\tMin Image Count: " << BLUE << details.capabilities.minImageCount << ENDC << std::endl;
			std::cout << "\tMax Image Count: " << BLUE << (details.capabilities.maxImageCount != 0 ? details.capabilities.maxImageCount + "" : "No Max" ) << ENDC << std::endl;
			if (details.capabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
				std::cout << "\tCurrent Extent: " << BLUE << "Determined by the extent if the swapchain targeting surface" << ENDC << std::endl;
			else
				std::cout << "\tCurrent Extent: " << BLUE << details.capabilities.currentExtent.width << "x" << details.capabilities.currentExtent.height << ENDC << " (WIDTHxHEIGHT)" << std::endl;
			if (details.capabilities.maxImageExtent.width == std::numeric_limits<uint32_t>::max())
				std::cout << "\tMin Image Extent: " << BLUE << "Determined by the extent if the swapchain targeting surface" << ENDC << std::endl;
			else
				std::cout << "\tMin Image Extent: " << BLUE << details.capabilities.maxImageExtent.width << "x" << details.capabilities.maxImageExtent.height << ENDC << " (WIDTHxHEIGHT)" << std::endl;
			if (details.capabilities.maxImageExtent.width == std::numeric_limits<uint32_t>::max())
				std::cout << "\tMax Image Extent: " << BLUE << "Determined by the extent if the swapchain targeting surface" << ENDC << std::endl;
			else
				std::cout << "\tMax Image Extent: " << BLUE << details.capabilities.maxImageExtent.width << "x" << details.capabilities.maxImageExtent.height << ENDC << " (WIDTHxHEIGHT)" << std::endl;
			std::cout << "\tMax Image Array Lenght: " << BLUE << details.capabilities.maxImageArrayLayers << ENDC << std::endl;
			std::cout << "\tFounded " << GREEN << formatCount << ENDC << " supported surface formats" << std::endl;  
			std::cout << "\tFounded " << GREEN << presentModeCount << ENDC << " supported surface modes" << std::endl << std::endl;  
		}
    return details;
}

VkSurfaceFormatKHR Vulkan42::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	for (const VkSurfaceFormatKHR& availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return availableFormat;
    }
	}
	return availableFormats[0];
}

VkPresentModeKHR Vulkan42::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	for (const VkPresentModeKHR& availablePresentMode : availablePresentModes) {
 	 if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
	}
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Vulkan42::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
	  return capabilities.currentExtent;
	} else {
	  int width, height;
	  glfwGetFramebufferSize(win.glfw_window, &width, &height);

	  VkExtent2D actualExtent = {
	      static_cast<uint32_t>(width),
	      static_cast<uint32_t>(height)
	  };

	  actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	  actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

	  return actualExtent;
	}
}

Vulkan42::~Vulkan42() {
  for (VkImageView imageView : swapChainImageViews) {
    vkDestroyImageView(device, imageView, nullptr);
  }
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
  vkDestroyDevice(device, nullptr);
  // vkDestroyInstance(instance, nullptr);
	std::cout << "Vulkan42" << " destroyed" << std::endl;
	// TODO (destructor)
}

std::ostream& operator<<(std::ostream& s, const Vulkan42& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

