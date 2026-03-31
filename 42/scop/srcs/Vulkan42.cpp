#include "Vulkan42.hpp"
#include "Scop.hpp"
#include "ShaderUtils.hpp"

#include <algorithm>
#include <cstring>
#include <limits>
#include <set>
#include <stdexcept>
#include <vector>

namespace scop {

// -------------------------------------------------------------------------- //
//  Validation layer debug callback                                            //
// -------------------------------------------------------------------------- //
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
    VkDebugUtilsMessageTypeFlagsEXT             type,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void*                                       /*userData*/)
{
  (void)type;
  if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    std::cerr << YELLOW << "[Vulkan] " << ENDC << callbackData->pMessage << std::endl;
  }
  return VK_FALSE;
}

// Proxy functions -- vkCreateDebugUtilsMessengerEXT is an extension function
// and must be loaded manually via vkGetInstanceProcAddr.
static VkResult createDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
    const VkAllocationCallbacks* allocator,
    VkDebugUtilsMessengerEXT* messenger)
{
  auto fn = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
  if (fn) return fn(instance, createInfo, allocator, messenger);
  return VK_ERROR_EXTENSION_NOT_PRESENT;
}

static void destroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT messenger,
    const VkAllocationCallbacks* allocator)
{
  auto fn = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
  if (fn) fn(instance, messenger, allocator);
}

// -------------------------------------------------------------------------- //
//  Queue family lookup                                                        //
// -------------------------------------------------------------------------- //
struct QueueFamilyIndices {
  uint32_t graphics = UINT32_MAX;
  uint32_t present  = UINT32_MAX;

  bool isComplete() const {
    return graphics != UINT32_MAX && present != UINT32_MAX;
  }
};

// -------------------------------------------------------------------------- //
//  Swapchain support query                                                    //
// -------------------------------------------------------------------------- //
struct SwapchainSupportDetails {
  VkSurfaceCapabilitiesKHR        capabilities{};
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR>   presentModes;
};

// -------------------------------------------------------------------------- //
//  Impl (pimpl)                                                               //
// -------------------------------------------------------------------------- //
struct Vulkan42::Impl {
  // Core Vulkan handles
  VkInstance               instance       = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
  VkSurfaceKHR             surface        = VK_NULL_HANDLE;
  VkPhysicalDevice         physicalDevice = VK_NULL_HANDLE;
  VkDevice                 device         = VK_NULL_HANDLE;
  VkQueue                  graphicsQueue  = VK_NULL_HANDLE;
  VkQueue                  presentQueue   = VK_NULL_HANDLE;
  QueueFamilyIndices       queueFamilies;

  // Swapchain
  VkSwapchainKHR             swapchain       = VK_NULL_HANDLE;
  VkFormat                   swapchainFormat = VK_FORMAT_UNDEFINED;
  VkExtent2D                 swapchainExtent = {0, 0};
  std::vector<VkImage>       swapchainImages;
  std::vector<VkImageView>   swapchainImageViews;

  // Depth buffer
  VkImage        depthImage       = VK_NULL_HANDLE;
  VkDeviceMemory depthImageMemory = VK_NULL_HANDLE;
  VkImageView    depthImageView   = VK_NULL_HANDLE;
  VkFormat       depthFormat      = VK_FORMAT_UNDEFINED;

  // Render pass
  VkRenderPass renderPass = VK_NULL_HANDLE;

  // Pipeline
  VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
  VkPipelineLayout      pipelineLayout      = VK_NULL_HANDLE;
  VkPipeline            graphicsPipeline    = VK_NULL_HANDLE;

  // Window reference
  Window*     window  = nullptr;
  int         width   = 0;
  int         height  = 0;

  // Data from the app
  RenderMode     mode    = RenderMode::Faces;
  Mesh           mesh;
  const Texture* texture = nullptr;

  bool enableValidation = false;
  bool inited           = false;

  // Validation layers we request
  static constexpr const char* validationLayers[] = {
    "VK_LAYER_KHRONOS_validation"
  };
  static constexpr uint32_t validationLayerCount = 1;

  // Device extensions we require
  static constexpr const char* deviceExtensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };
  static constexpr uint32_t deviceExtensionCount = 1;

  // --- Init methods ---
  void createInstance();
  void setupDebugMessenger();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSwapchain();
  void createImageViews();
  void createRenderPass();
  void createDepthResources();
  void createDescriptorSetLayout();
  void createGraphicsPipeline();
  void cleanup();
  void cleanupSwapchain();

  // Shader helpers
  VkShaderModule createShaderModule(const std::vector<char>& code) const;

  // Helpers
  bool                     checkValidationLayerSupport() const;
  std::vector<const char*> getRequiredExtensions() const;
  QueueFamilyIndices       findQueueFamilies(VkPhysicalDevice dev) const;
  bool                     checkDeviceExtensionSupport(VkPhysicalDevice dev) const;
  bool                     isDeviceSuitable(VkPhysicalDevice dev) const;
  SwapchainSupportDetails  querySwapchainSupport(VkPhysicalDevice dev) const;

  static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available);
  static VkPresentModeKHR   chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available);
  VkExtent2D                chooseSwapExtent(const VkSurfaceCapabilitiesKHR& caps) const;

  // Image / memory helpers (reused for depth buffer and textures)
  uint32_t    findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
  void        createImage(uint32_t w, uint32_t h, VkFormat fmt, VkImageTiling tiling,
                          VkImageUsageFlags usage, VkMemoryPropertyFlags memProps,
                          VkImage& image, VkDeviceMemory& memory) const;
  VkImageView createImageView(VkImage image, VkFormat fmt, VkImageAspectFlags aspect) const;
  VkFormat    findSupportedFormat(const std::vector<VkFormat>& candidates,
                                 VkImageTiling tiling, VkFormatFeatureFlags features) const;
  VkFormat    findDepthFormat() const;

  static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info);
};

// -------------------------------------------------------------------------- //
//  Instance creation                                                          //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::createInstance() {
  enableValidation = SCOP_DEBUG;

  if (enableValidation && !checkValidationLayerSupport()) {
    std::cerr << YELLOW << "[Vulkan] Validation layers requested but not available. "
              << "Continuing without them." << ENDC << std::endl;
    enableValidation = false;
  }

  VkApplicationInfo appInfo{};
  appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName   = "42 Scop";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName        = "Pulga Engine";
  appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion         = VK_API_VERSION_1_0;

  auto extensions = getRequiredExtensions();

  VkInstanceCreateInfo createInfo{};
  createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo        = &appInfo;
  createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  // Attach validation layers + debug messenger for instance creation/destruction
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if (enableValidation) {
    createInfo.enabledLayerCount   = validationLayerCount;
    createInfo.ppEnabledLayerNames = validationLayers;

    populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = &debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext             = nullptr;
  }

  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    throw std::runtime_error("Failed to create Vulkan instance");

  std::cout << GREEN << "[Vulkan] Instance created"
            << (enableValidation ? " (validation ON)" : "") << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Debug messenger                                                            //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT& info)
{
  info = {};
  info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  info.messageSeverity  = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  info.messageType      = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  info.pfnUserCallback  = debugCallback;
}

void Vulkan42::Impl::setupDebugMessenger() {
  if (!enableValidation) return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  populateDebugMessengerCreateInfo(createInfo);

  if (createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    throw std::runtime_error("Failed to set up Vulkan debug messenger");

  std::cout << GREEN << "[Vulkan] Debug messenger ready" << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Surface                                                                    //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::createSurface() {
  if (glfwCreateWindowSurface(instance, window->glfw_window, nullptr, &surface) != VK_SUCCESS)
    throw std::runtime_error("Failed to create window surface");

  std::cout << GREEN << "[Vulkan] Surface created" << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Physical device selection                                                  //
// -------------------------------------------------------------------------- //
QueueFamilyIndices Vulkan42::Impl::findQueueFamilies(VkPhysicalDevice dev) const {
  QueueFamilyIndices indices;

  uint32_t count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(dev, &count, nullptr);
  std::vector<VkQueueFamilyProperties> families(count);
  vkGetPhysicalDeviceQueueFamilyProperties(dev, &count, families.data());

  for (uint32_t i = 0; i < count; ++i) {
    if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      indices.graphics = i;

    VkBool32 presentSupport = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, surface, &presentSupport);
    if (presentSupport)
      indices.present = i;

    if (indices.isComplete()) break;
  }
  return indices;
}

bool Vulkan42::Impl::checkDeviceExtensionSupport(VkPhysicalDevice dev) const {
  uint32_t count = 0;
  vkEnumerateDeviceExtensionProperties(dev, nullptr, &count, nullptr);
  std::vector<VkExtensionProperties> available(count);
  vkEnumerateDeviceExtensionProperties(dev, nullptr, &count, available.data());

  for (uint32_t i = 0; i < deviceExtensionCount; ++i) {
    bool found = false;
    for (const auto& ext : available) {
      if (std::strcmp(deviceExtensions[i], ext.extensionName) == 0) {
        found = true;
        break;
      }
    }
    if (!found) return false;
  }
  return true;
}

bool Vulkan42::Impl::isDeviceSuitable(VkPhysicalDevice dev) const {
  QueueFamilyIndices indices = findQueueFamilies(dev);
  if (!indices.isComplete()) return false;
  if (!checkDeviceExtensionSupport(dev)) return false;

  // Check that the surface supports at least one format and present mode
  uint32_t formatCount = 0, modeCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &formatCount, nullptr);
  vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &modeCount, nullptr);

  return formatCount > 0 && modeCount > 0;
}

void Vulkan42::Impl::pickPhysicalDevice() {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  if (deviceCount == 0)
    throw std::runtime_error("No GPUs with Vulkan support found");

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  // Prefer discrete GPU, fall back to any suitable device
  for (const auto& dev : devices) {
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(dev, &props);

    if (isDeviceSuitable(dev) && props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      physicalDevice = dev;
      std::cout << GREEN << "[Vulkan] GPU: " << props.deviceName
                << " (discrete)" << ENDC << std::endl;
      break;
    }
  }

  if (physicalDevice == VK_NULL_HANDLE) {
    for (const auto& dev : devices) {
      if (isDeviceSuitable(dev)) {
        physicalDevice = dev;
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(dev, &props);
        std::cout << GREEN << "[Vulkan] GPU: " << props.deviceName << ENDC << std::endl;
        break;
      }
    }
  }

  if (physicalDevice == VK_NULL_HANDLE)
    throw std::runtime_error("No suitable GPU found");

  queueFamilies = findQueueFamilies(physicalDevice);
}

// -------------------------------------------------------------------------- //
//  Logical device + queues                                                    //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::createLogicalDevice() {
  // We may have the same family for graphics and present -- use a set to deduplicate
  std::set<uint32_t> uniqueFamilies = {queueFamilies.graphics, queueFamilies.present};

  float queuePriority = 1.0f;
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  for (uint32_t family : uniqueFamilies) {
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = family;
    queueInfo.queueCount       = 1;
    queueInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};
  // No special features needed for the mandatory part

  VkDeviceCreateInfo createInfo{};
  createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount    = static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos       = queueCreateInfos.data();
  createInfo.pEnabledFeatures        = &deviceFeatures;
  createInfo.enabledExtensionCount   = deviceExtensionCount;
  createInfo.ppEnabledExtensionNames = deviceExtensions;

  // Validation layers on the device (ignored by modern drivers, but keeps older ones happy)
  if (enableValidation) {
    createInfo.enabledLayerCount   = validationLayerCount;
    createInfo.ppEnabledLayerNames = validationLayers;
  } else {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
    throw std::runtime_error("Failed to create logical device");

  vkGetDeviceQueue(device, queueFamilies.graphics, 0, &graphicsQueue);
  vkGetDeviceQueue(device, queueFamilies.present,  0, &presentQueue);

  std::cout << GREEN << "[Vulkan] Logical device created (graphics queue: "
            << queueFamilies.graphics << ", present queue: "
            << queueFamilies.present << ")" << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Swapchain                                                                  //
// -------------------------------------------------------------------------- //
SwapchainSupportDetails Vulkan42::Impl::querySwapchainSupport(VkPhysicalDevice dev) const {
  SwapchainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dev, surface, &details.capabilities);

  uint32_t formatCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &formatCount, nullptr);
  if (formatCount > 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &formatCount, details.formats.data());
  }

  uint32_t modeCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &modeCount, nullptr);
  if (modeCount > 0) {
    details.presentModes.resize(modeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &modeCount, details.presentModes.data());
  }

  return details;
}

VkSurfaceFormatKHR Vulkan42::Impl::chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& available)
{
  // Prefer SRGB with B8G8R8A8 layout
  for (const auto& fmt : available) {
    if (fmt.format == VK_FORMAT_B8G8R8A8_SRGB &&
        fmt.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      return fmt;
  }
  return available[0];
}

VkPresentModeKHR Vulkan42::Impl::chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR>& available)
{
  // Mailbox = triple buffering (low latency, no tearing). Fall back to FIFO (always available).
  for (const auto& mode : available) {
    if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
      return mode;
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Vulkan42::Impl::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& caps) const {
  // If currentExtent is not the special value 0xFFFFFFFF, the surface size is fixed
  if (caps.currentExtent.width != std::numeric_limits<uint32_t>::max())
    return caps.currentExtent;

  // Otherwise, pick the window's framebuffer size clamped to the surface limits
  int fbWidth = 0, fbHeight = 0;
  glfwGetFramebufferSize(window->glfw_window, &fbWidth, &fbHeight);

  VkExtent2D extent = {
    static_cast<uint32_t>(fbWidth),
    static_cast<uint32_t>(fbHeight)
  };
  extent.width  = std::clamp(extent.width,  caps.minImageExtent.width,  caps.maxImageExtent.width);
  extent.height = std::clamp(extent.height, caps.minImageExtent.height, caps.maxImageExtent.height);
  return extent;
}

void Vulkan42::Impl::createSwapchain() {
  SwapchainSupportDetails support = querySwapchainSupport(physicalDevice);

  VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(support.formats);
  VkPresentModeKHR   presentMode   = chooseSwapPresentMode(support.presentModes);
  VkExtent2D         extent        = chooseSwapExtent(support.capabilities);

  // Request one more image than the minimum for triple buffering headroom
  uint32_t imageCount = support.capabilities.minImageCount + 1;
  if (support.capabilities.maxImageCount > 0 && imageCount > support.capabilities.maxImageCount)
    imageCount = support.capabilities.maxImageCount;

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface          = surface;
  createInfo.minImageCount    = imageCount;
  createInfo.imageFormat      = surfaceFormat.format;
  createInfo.imageColorSpace  = surfaceFormat.colorSpace;
  createInfo.imageExtent      = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  // Handle queue family sharing if graphics != present
  uint32_t familyIndices[] = {queueFamilies.graphics, queueFamilies.present};
  if (queueFamilies.graphics != queueFamilies.present) {
    createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices   = familyIndices;
  } else {
    createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
  }

  createInfo.preTransform   = support.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode    = presentMode;
  createInfo.clipped        = VK_TRUE;
  createInfo.oldSwapchain   = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) != VK_SUCCESS)
    throw std::runtime_error("Failed to create swapchain");

  // Retrieve the swapchain images (Vulkan may have created more than we asked for)
  vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
  swapchainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());

  swapchainFormat = surfaceFormat.format;
  swapchainExtent = extent;

  std::cout << GREEN << "[Vulkan] Swapchain created ("
            << extent.width << "x" << extent.height << ", "
            << imageCount << " images)" << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Image views                                                                //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::createImageViews() {
  swapchainImageViews.resize(swapchainImages.size());

  for (size_t i = 0; i < swapchainImages.size(); ++i) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image    = swapchainImages[i];
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format   = swapchainFormat;

    viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    viewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel   = 0;
    viewInfo.subresourceRange.levelCount     = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount     = 1;

    if (vkCreateImageView(device, &viewInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS)
      throw std::runtime_error("Failed to create swapchain image view");
  }

  std::cout << GREEN << "[Vulkan] " << swapchainImageViews.size()
            << " image views created" << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Image / memory helpers                                                     //
// -------------------------------------------------------------------------- //
uint32_t Vulkan42::Impl::findMemoryType(uint32_t typeFilter,
                                         VkMemoryPropertyFlags properties) const
{
  VkPhysicalDeviceMemoryProperties memProps;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);

  for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i) {
    if ((typeFilter & (1 << i)) &&
        (memProps.memoryTypes[i].propertyFlags & properties) == properties)
      return i;
  }
  throw std::runtime_error("Failed to find suitable memory type");
}

void Vulkan42::Impl::createImage(uint32_t w, uint32_t h, VkFormat fmt,
                                  VkImageTiling tiling, VkImageUsageFlags usage,
                                  VkMemoryPropertyFlags memProps,
                                  VkImage& image, VkDeviceMemory& memory) const
{
  VkImageCreateInfo imageInfo{};
  imageInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType     = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width  = w;
  imageInfo.extent.height = h;
  imageInfo.extent.depth  = 1;
  imageInfo.mipLevels     = 1;
  imageInfo.arrayLayers   = 1;
  imageInfo.format        = fmt;
  imageInfo.tiling        = tiling;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage         = usage;
  imageInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
    throw std::runtime_error("Failed to create image");

  VkMemoryRequirements memReqs;
  vkGetImageMemoryRequirements(device, image, &memReqs);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize  = memReqs.size;
  allocInfo.memoryTypeIndex = findMemoryType(memReqs.memoryTypeBits, memProps);

  if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
    throw std::runtime_error("Failed to allocate image memory");

  vkBindImageMemory(device, image, memory, 0);
}

VkImageView Vulkan42::Impl::createImageView(VkImage image, VkFormat fmt,
                                              VkImageAspectFlags aspect) const
{
  VkImageViewCreateInfo viewInfo{};
  viewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image                           = image;
  viewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format                          = fmt;
  viewInfo.subresourceRange.aspectMask     = aspect;
  viewInfo.subresourceRange.baseMipLevel   = 0;
  viewInfo.subresourceRange.levelCount     = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount     = 1;

  VkImageView view;
  if (vkCreateImageView(device, &viewInfo, nullptr, &view) != VK_SUCCESS)
    throw std::runtime_error("Failed to create image view");
  return view;
}

VkFormat Vulkan42::Impl::findSupportedFormat(const std::vector<VkFormat>& candidates,
                                              VkImageTiling tiling,
                                              VkFormatFeatureFlags features) const
{
  for (VkFormat fmt : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, fmt, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features)
      return fmt;
    if (tiling == VK_IMAGE_TILING_OPTIMAL &&
        (props.optimalTilingFeatures & features) == features)
      return fmt;
  }
  throw std::runtime_error("Failed to find supported format");
}

VkFormat Vulkan42::Impl::findDepthFormat() const {
  return findSupportedFormat(
    {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
    VK_IMAGE_TILING_OPTIMAL,
    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
  );
}

// -------------------------------------------------------------------------- //
//  Depth resources                                                            //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::createDepthResources() {
  depthFormat = findDepthFormat();

  createImage(
    swapchainExtent.width, swapchainExtent.height,
    depthFormat, VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    depthImage, depthImageMemory
  );

  depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

  std::cout << GREEN << "[Vulkan] Depth buffer created" << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Render pass                                                                //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::createRenderPass() {
  // Color attachment -- swapchain image
  VkAttachmentDescription colorAttachment{};
  colorAttachment.format         = swapchainFormat;
  colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  // Depth attachment
  VkAttachmentDescription depthAttachment{};
  depthAttachment.format         = depthFormat;
  depthAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference colorRef{};
  colorRef.attachment = 0;
  colorRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthRef{};
  depthRef.attachment = 1;
  depthRef.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount    = 1;
  subpass.pColorAttachments       = &colorRef;
  subpass.pDepthStencilAttachment = &depthRef;

  // Ensure the swapchain image is available before we start writing to it
  VkSubpassDependency dependency{};
  dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass    = 0;
  dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                           | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                           | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
                           | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  VkAttachmentDescription attachments[] = {colorAttachment, depthAttachment};

  VkRenderPassCreateInfo rpInfo{};
  rpInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  rpInfo.attachmentCount = 2;
  rpInfo.pAttachments    = attachments;
  rpInfo.subpassCount    = 1;
  rpInfo.pSubpasses      = &subpass;
  rpInfo.dependencyCount = 1;
  rpInfo.pDependencies   = &dependency;

  if (vkCreateRenderPass(device, &rpInfo, nullptr, &renderPass) != VK_SUCCESS)
    throw std::runtime_error("Failed to create render pass");

  std::cout << GREEN << "[Vulkan] Render pass created" << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Descriptor set layout                                                      //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::createDescriptorSetLayout() {
  // Binding 0: UBO (vertex shader) -- MVP matrices
  VkDescriptorSetLayoutBinding uboBinding{};
  uboBinding.binding            = 0;
  uboBinding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboBinding.descriptorCount    = 1;
  uboBinding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;

  // Binding 1: combined image sampler (fragment shader) -- texture
  VkDescriptorSetLayoutBinding samplerBinding{};
  samplerBinding.binding            = 1;
  samplerBinding.descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  samplerBinding.descriptorCount    = 1;
  samplerBinding.stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;

  VkDescriptorSetLayoutBinding bindings[] = {uboBinding, samplerBinding};

  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount  = 2;
  layoutInfo.pBindings     = bindings;

  if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
    throw std::runtime_error("Failed to create descriptor set layout");

  std::cout << GREEN << "[Vulkan] Descriptor set layout created" << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Shader modules                                                             //
// -------------------------------------------------------------------------- //
VkShaderModule Vulkan42::Impl::createShaderModule(const std::vector<char>& code) const {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());

  VkShaderModule module;
  if (vkCreateShaderModule(device, &createInfo, nullptr, &module) != VK_SUCCESS)
    throw std::runtime_error("Failed to create shader module");
  return module;
}

// -------------------------------------------------------------------------- //
//  Graphics pipeline                                                          //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::createGraphicsPipeline() {
  // --- Load SPIR-V ---
  auto vertCode = readSpirv("assets/shaders/mesh.vert.spv");
  auto fragCode = readSpirv("assets/shaders/mesh.frag.spv");

  VkShaderModule vertModule = createShaderModule(vertCode);
  VkShaderModule fragModule = createShaderModule(fragCode);

  VkPipelineShaderStageCreateInfo vertStage{};
  vertStage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertStage.stage  = VK_SHADER_STAGE_VERTEX_BIT;
  vertStage.module = vertModule;
  vertStage.pName  = "main";

  VkPipelineShaderStageCreateInfo fragStage{};
  fragStage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragStage.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragStage.module = fragModule;
  fragStage.pName  = "main";

  VkPipelineShaderStageCreateInfo shaderStages[] = {vertStage, fragStage};

  // --- Vertex input ---
  // Must match struct Vertex { Vec3 pos; Vec3 normal; Vec2 uv; uint32_t faceId; }
  VkVertexInputBindingDescription bindingDesc{};
  bindingDesc.binding   = 0;
  bindingDesc.stride    = sizeof(Vertex);
  bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  VkVertexInputAttributeDescription attrDescs[4]{};
  // location 0: pos (vec3)
  attrDescs[0].binding  = 0;
  attrDescs[0].location = 0;
  attrDescs[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
  attrDescs[0].offset   = offsetof(Vertex, pos);
  // location 1: normal (vec3)
  attrDescs[1].binding  = 0;
  attrDescs[1].location = 1;
  attrDescs[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
  attrDescs[1].offset   = offsetof(Vertex, normal);
  // location 2: uv (vec2)
  attrDescs[2].binding  = 0;
  attrDescs[2].location = 2;
  attrDescs[2].format   = VK_FORMAT_R32G32_SFLOAT;
  attrDescs[2].offset   = offsetof(Vertex, uv);
  // location 3: faceId (uint)
  attrDescs[3].binding  = 0;
  attrDescs[3].location = 3;
  attrDescs[3].format   = VK_FORMAT_R32_UINT;
  attrDescs[3].offset   = offsetof(Vertex, faceId);

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount    = 1;
  vertexInputInfo.pVertexBindingDescriptions       = &bindingDesc;
  vertexInputInfo.vertexAttributeDescriptionCount  = 4;
  vertexInputInfo.pVertexAttributeDescriptions     = attrDescs;

  // --- Input assembly ---
  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

  // --- Dynamic viewport and scissor ---
  VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamicState{};
  dynamicState.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.dynamicStateCount = 2;
  dynamicState.pDynamicStates    = dynamicStates;

  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.scissorCount  = 1;

  // --- Rasterizer ---
  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType       = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth   = 1.0f;
  rasterizer.cullMode    = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace   = VK_FRONT_FACE_COUNTER_CLOCKWISE;

  // --- Multisampling (disabled) ---
  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  // --- Depth / stencil ---
  VkPipelineDepthStencilStateCreateInfo depthStencil{};
  depthStencil.sType            = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencil.depthTestEnable  = VK_TRUE;
  depthStencil.depthWriteEnable = VK_TRUE;
  depthStencil.depthCompareOp   = VK_COMPARE_OP_LESS;

  // --- Color blending (no blending, just write) ---
  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
                                      | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

  VkPipelineColorBlendStateCreateInfo colorBlending{};
  colorBlending.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments    = &colorBlendAttachment;

  // --- Push constants (blend factor for fragment shader) ---
  VkPushConstantRange pushRange{};
  pushRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  pushRange.offset     = 0;
  pushRange.size       = sizeof(PushConstants);

  // --- Pipeline layout ---
  VkPipelineLayoutCreateInfo layoutInfo{};
  layoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  layoutInfo.setLayoutCount         = 1;
  layoutInfo.pSetLayouts            = &descriptorSetLayout;
  layoutInfo.pushConstantRangeCount = 1;
  layoutInfo.pPushConstantRanges    = &pushRange;

  if (vkCreatePipelineLayout(device, &layoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    throw std::runtime_error("Failed to create pipeline layout");

  // --- Graphics pipeline ---
  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount          = 2;
  pipelineInfo.pStages             = shaderStages;
  pipelineInfo.pVertexInputState   = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState      = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState   = &multisampling;
  pipelineInfo.pDepthStencilState  = &depthStencil;
  pipelineInfo.pColorBlendState    = &colorBlending;
  pipelineInfo.pDynamicState       = &dynamicState;
  pipelineInfo.layout              = pipelineLayout;
  pipelineInfo.renderPass          = renderPass;
  pipelineInfo.subpass             = 0;

  if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo,
                                nullptr, &graphicsPipeline) != VK_SUCCESS)
    throw std::runtime_error("Failed to create graphics pipeline");

  // Shader modules can be destroyed after pipeline creation
  vkDestroyShaderModule(device, fragModule, nullptr);
  vkDestroyShaderModule(device, vertModule, nullptr);

  std::cout << GREEN << "[Vulkan] Graphics pipeline created" << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Helpers                                                                    //
// -------------------------------------------------------------------------- //
bool Vulkan42::Impl::checkValidationLayerSupport() const {
  uint32_t layerCount = 0;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> available(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, available.data());

  for (uint32_t i = 0; i < validationLayerCount; ++i) {
    bool found = false;
    for (const auto& layer : available) {
      if (std::strcmp(validationLayers[i], layer.layerName) == 0) {
        found = true;
        break;
      }
    }
    if (!found) return false;
  }
  return true;
}

std::vector<const char*> Vulkan42::Impl::getRequiredExtensions() const {
  // GLFW tells us which instance extensions it needs for surface creation
  uint32_t glfwExtCount = 0;
  const char** glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtCount);

  std::vector<const char*> extensions(glfwExts, glfwExts + glfwExtCount);

  if (enableValidation)
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  return extensions;
}

// -------------------------------------------------------------------------- //
//  Cleanup                                                                    //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::cleanupSwapchain() {
  if (depthImageView != VK_NULL_HANDLE) {
    vkDestroyImageView(device, depthImageView, nullptr);
    depthImageView = VK_NULL_HANDLE;
  }
  if (depthImage != VK_NULL_HANDLE) {
    vkDestroyImage(device, depthImage, nullptr);
    depthImage = VK_NULL_HANDLE;
  }
  if (depthImageMemory != VK_NULL_HANDLE) {
    vkFreeMemory(device, depthImageMemory, nullptr);
    depthImageMemory = VK_NULL_HANDLE;
  }

  for (auto view : swapchainImageViews)
    vkDestroyImageView(device, view, nullptr);
  swapchainImageViews.clear();

  if (swapchain != VK_NULL_HANDLE) {
    vkDestroySwapchainKHR(device, swapchain, nullptr);
    swapchain = VK_NULL_HANDLE;
  }
  swapchainImages.clear();
}

void Vulkan42::Impl::cleanup() {
  // Destroy in reverse creation order
  cleanupSwapchain();

  if (graphicsPipeline != VK_NULL_HANDLE) {
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    graphicsPipeline = VK_NULL_HANDLE;
  }
  if (pipelineLayout != VK_NULL_HANDLE) {
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    pipelineLayout = VK_NULL_HANDLE;
  }
  if (descriptorSetLayout != VK_NULL_HANDLE) {
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
    descriptorSetLayout = VK_NULL_HANDLE;
  }
  if (renderPass != VK_NULL_HANDLE) {
    vkDestroyRenderPass(device, renderPass, nullptr);
    renderPass = VK_NULL_HANDLE;
  }

  if (device != VK_NULL_HANDLE) {
    vkDestroyDevice(device, nullptr);
    device = VK_NULL_HANDLE;
  }
  if (surface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(instance, surface, nullptr);
    surface = VK_NULL_HANDLE;
  }
  if (debugMessenger != VK_NULL_HANDLE) {
    destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    debugMessenger = VK_NULL_HANDLE;
  }
  if (instance != VK_NULL_HANDLE) {
    vkDestroyInstance(instance, nullptr);
    instance = VK_NULL_HANDLE;
  }
}

// -------------------------------------------------------------------------- //
//  Public interface                                                           //
// -------------------------------------------------------------------------- //
Vulkan42::Vulkan42()  : impl(new Impl) {}
Vulkan42::~Vulkan42() {
  if (impl) {
    impl->cleanup();
    delete impl;
  }
}

void Vulkan42::init(Window& window) {
  impl->window = &window;
  impl->width  = window.getWidth();
  impl->height = window.getHeight();

  impl->createInstance();
  impl->setupDebugMessenger();
  impl->createSurface();
  impl->pickPhysicalDevice();
  impl->createLogicalDevice();
  impl->createSwapchain();
  impl->createImageViews();
  impl->createDepthResources();
  impl->createRenderPass();
  impl->createDescriptorSetLayout();
  impl->createGraphicsPipeline();
  // Next steps: createFramebuffers, command pool, buffers, descriptors, sync, draw loop

  impl->inited = true;
  std::cout << GREEN << "[Vulkan] Initialized " << impl->width << "x" << impl->height
            << ENDC << std::endl;
}

void Vulkan42::resize(int width, int height) {
  impl->width  = width;
  impl->height = height;
  // TODO: recreate swapchain
}

void Vulkan42::setMesh(const Mesh& mesh) {
  impl->mesh = mesh;
  // TODO: upload vertex/index buffers to GPU
}

void Vulkan42::setTexture(const Texture* texture) {
  impl->texture = texture;
  // TODO: create VkImage + sampler
}

void Vulkan42::setRenderMode(RenderMode mode) {
  impl->mode = mode;
}

void Vulkan42::draw(const FrameState& /*state*/) {
  if (!impl->inited) return;
  // TODO: update UBOs, record command buffer, submit, present
}

void Vulkan42::waitIdle() {
  if (impl->device != VK_NULL_HANDLE)
    vkDeviceWaitIdle(impl->device);
}

std::ostream& operator<<(std::ostream& s, const Vulkan42&) {
  s << "Vulkan42 Renderer";
  return s;
}

} // namespace scop
