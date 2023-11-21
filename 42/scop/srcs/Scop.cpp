//***************************//
//*Template by pulgamecanica*//
//***************************//
#include "Scop.hpp"
#include "Window.hpp"
#include "utils.hpp"

using namespace scop;

// Wrappers to handle event callbacks
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    static_cast<scop::Scop *>(glfwGetWindowUserPointer(window))->keyEvent(key, action, mods);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    static_cast<scop::Scop *>(glfwGetWindowUserPointer(window))->mouseButtonEvent(button, action, mods);
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    static_cast<scop::Scop *>(glfwGetWindowUserPointer(window))->mouseMoveEvent(xpos, ypos);
}
void window_size_callback(GLFWwindow* window, int width, int height) {
    static_cast<scop::Scop *>(glfwGetWindowUserPointer(window))->resize(width, height);
}

Scop::Scop() {
  win = new Window(1000, 650);
  status = ScopStatus::Menu;
  physicalDevice = VK_NULL_HANDLE;
  // NOTICE:  Change the conventional pointer to my custom pointer
  //          Requires a static_cast<Scop> in order to retreive it!
  glfwSetWindowUserPointer(win->glfw_window, this);
  // Use the Wrappers which will triger scopObj.***Event()
  glfwSetKeyCallback(win->glfw_window, key_callback);
  glfwSetCursorPosCallback(win->glfw_window, cursor_position_callback);
  glfwSetMouseButtonCallback(win->glfw_window, mouse_button_callback);
  glfwSetWindowSizeCallback(win->glfw_window, window_size_callback);

 
  initVulkan();

  created_at = gettimeofday_ms();
  updated_at = gettimeofday_ms();
}

Scop::~Scop() {
  // vkDestroyDevice(physicalDevice.data(), nullptr);
  vkDestroyInstance(instance, nullptr);
  delete win;
  std::cout << "Scop" << " destroyed" << std::endl;
}

void Scop::initVulkan() {
  createVkInstance();
  pickPhysicalDevice();
}

void  Scop::createVkInstance() {
  VkApplicationInfo appInfo{};
  // ? -> What is sType?
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Scop";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  // ? -> What is Extansions? What is an extension?
  // Is it extensions for the shaders and the shaders languages?
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;

  createInfo.enabledLayerCount = 0;

  // VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
  // Nice, we have a VkInstance!!
  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }
}

bool Scop::isDeviceSuitable(VKDevice device) {
  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;

  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
  std::cout << "Device\t Name: " << BLUE << deviceProperties.deviceName << ENDC;
  std::cout << "\t Type: " << BLUE << (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER ? "Other" 
    : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "Integrated with Host"
    : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "Separated connected to Host" 
    : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU ? "Virtual (Node || Environment)" 
    : deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU ? "Running on the same processor" : "") << ENDC << std::endl;
  // Check ALL the Features and Properties that will be needed
  // - For now:
  // Defined Phisical Device Type
  // Geometry Shader
  return deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_OTHER &&
         deviceFeatures.geometryShader;
}

void  Scop::pickPhysicalDevice() {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }
  std::vector<VKDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  for (const VKDevice & device : devices) {
    if (isDeviceSuitable(device)) {
      physicalDevice = device;
      break;
    }
  }
  if (physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }
}

// uint32_t findQueueFamilies(VkPhysicalDevice device) {
//     // Logic to find graphics queue family
// }


void Scop::resize(int width, int height) {
  win->resizeWindow(width, height);
}

void Scop::mouseButtonEvent(int key, int action, int mods) {
  std::cout << "Mouse: " << GREEN << key << ENDC << " Action: " << GREEN << action << ENDC << " Mods: " << GREEN << mods << ENDC << std::endl;  

}
void Scop::mouseMoveEvent(double xpos, double ypos) {
  std::cout << "MouseMove: (" << xpos << "," << ypos << ")" << std::endl;
}

void Scop::keyEvent(int key, int action, int mods) {
  std::cout << "Key: " << GREEN << key << ENDC << " Action: " << GREEN << action << ENDC << " Mods: " << GREEN << mods << ENDC << std::endl;  
}

ScopStatus Scop::getStatus() const { return (status); };

bool Scop::checkStatus(ScopStatus status) const { return (this->status == status); }

void Scop::runScop() {
  int fps = 0;
  while (!glfwWindowShouldClose(win->glfw_window)) {



      /* Render here */
      glClear(GL_COLOR_BUFFER_BIT);

      /* Swap front and back buffers */
      glfwSwapBuffers(win->glfw_window);

      /* Poll for and process events */
      glfwPollEvents();
      
      if (timestamp_in_ms(updated_at) < 1000) {
          fps++;
          continue;
      }
      updated_at = gettimeofday_ms();
      std::cout << "[" << GREEN << updated_at - created_at << ENDC << "] " << fps << " fps " << YELLOW << (status == ScopStatus::Menu ? "Menu" : status == ScopStatus::Settings ? "Settings" : "Rendering") << ENDC << std::endl;
      fps = 0;
  }
}

std::ostream& scop::operator<<(std::ostream& s, const Scop& param) {
  // s << param.CONST_METHOD()
  (void)param;
  return (s);
}

