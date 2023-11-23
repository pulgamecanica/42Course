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
	  std::cout << "Created Vulkan Instance <"<< BLUE << application_name << ENDC << ">" << std::endl;
	  std::cout << " * " << GREEN << "ApplicationInfo" << ENDC << ":\t\t" << "version(" << BLUE << VK_MAKE_VERSION(1, 0, 0) << ENDC << ")" << std::endl;
	  std::cout << " * " << GREEN << "ApplicationInfo" << ENDC << ":\t\t" << "engine(" << BLUE << appInfo.pEngineName << " - version " << VK_MAKE_VERSION(1, 0, 0) << ENDC << ")" << std::endl;
	  std::cout << " * " << GREEN << "ApplicationInfo" << ENDC << ":\t\t" << "apiVersion("<< BLUE  << VK_API_VERSION_1_0 << ENDC << ")" << std::endl;
	  std::cout << " * " << GREEN << "VkInstanceCreateInfo" << ENDC << ":\t" << "Enabled Extension Count(" << BLUE << glfwExtensionCount << ENDC << ")" << std::endl;
	  std::cout << " * " << GREEN << "VkInstanceCreateInfo" << ENDC << ":\t" << "Extension names:" << std::endl;
	  for (uint32_t i = 0; i < glfwExtensionCount; ++i)
	  	std::cout << " *\t\t\t\t - " << BLUE << glfwExtensions[i] << ENDC << std::endl;
	  std::cout << " * " << GREEN << "VkInstanceCreateInfo" << ENDC << ":\t" << "Enabled Layer Count(" << BLUE << createInfo.enabledLayerCount << ENDC << ")" << std::endl;
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
  	std::cout << "\t Name: " << BLUE << deviceProperties.deviceName << ENDC;
	  std::cout << "\t Type: " << BLUE << (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER ? "Other" 
	    : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "Integrated with Host"
	    : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "Separated connected to Host" 
	    : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU ? "Virtual (Node || Environment)" 
	    : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU ? "Running on the same processor" : "") << ENDC << std::endl;
  }
  //	Features and Properties that will be needed
  //	Known Phisical Device Type
  //	Geometry Shader
  return deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_OTHER &&
         deviceFeatures.geometryShader &&
         indices.isComplete() &&
         extensionsSupported;
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
  std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

  float queuePriority = 1.0f;
  for (uint32_t queueFamily : uniqueQueueFamilies) {
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamily;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back(queueCreateInfo);
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


Vulkan42::~Vulkan42() {
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

