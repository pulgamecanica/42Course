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

// We allow up to MAX_FRAMES_IN_FLIGHT frames to be processed concurrently by the
// GPU/CPU. Two is the textbook choice: while frame N is on the GPU, the CPU can
// already build frame N+1.
static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

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
  std::vector<VkFramebuffer> swapchainFramebuffers;

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

  // Commands
  VkCommandPool                 commandPool = VK_NULL_HANDLE;
  std::vector<VkCommandBuffer>  commandBuffers; // size = MAX_FRAMES_IN_FLIGHT

  // Synchronization
  //  - imageAvailable: per frame-in-flight (signaled by acquire, waited by submit)
  //  - renderFinished: per swapchain image (signaled by submit, waited by present).
  //    Per-image is required because the present queue may still be using the
  //    semaphore from a previous frame when we try to reuse it.
  //  - inFlightFences: per frame-in-flight (lets the CPU reuse the frame slot)
  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence>     inFlightFences;
  uint32_t                 currentFrame = 0;
  bool                     framebufferResized = false;

  // Vertex / index buffers (device-local, uploaded via staging buffer)
  VkBuffer       vertexBuffer       = VK_NULL_HANDLE;
  VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
  VkBuffer       indexBuffer        = VK_NULL_HANDLE;
  VkDeviceMemory indexBufferMemory  = VK_NULL_HANDLE;
  uint32_t       indexCount         = 0;
  bool           meshUploaded       = false;

  // Uniform buffers (one per frame in flight, host-visible + coherent, persistently mapped)
  std::vector<VkBuffer>       uniformBuffers;
  std::vector<VkDeviceMemory> uniformBuffersMemory;
  std::vector<void*>          uniformBuffersMapped;

  // Texture (real image + sampler; falls back to a 1x1 white pixel when none provided)
  VkImage        textureImage       = VK_NULL_HANDLE;
  VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
  VkImageView    textureImageView   = VK_NULL_HANDLE;
  VkSampler      textureSampler     = VK_NULL_HANDLE;

  // Descriptors
  VkDescriptorPool             descriptorPool = VK_NULL_HANDLE;
  std::vector<VkDescriptorSet> descriptorSets; // size = MAX_FRAMES_IN_FLIGHT

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
  void createFramebuffers();
  void createCommandPool();
  void createCommandBuffers();
  void createSyncObjects();
  void createUniformBuffers();
  void createFallbackTexture();
  void createDescriptorPool();
  void createDescriptorSets();
  void writeDescriptorSets();
  void cleanup();
  void cleanupSwapchain();
  void destroyTexture();
  void destroyMeshBuffers();

  void recreateSwapchain();

  // Per-frame work
  void recordCommandBuffer(VkCommandBuffer cb, uint32_t imageIndex, float blendFactor);
  void updateUniformBuffer(uint32_t frame, const FrameState& state);

  // Asset upload helpers
  void uploadMesh(const Mesh& mesh);
  void uploadTexture(const Texture& tex);

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

  // Buffer helpers
  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags props,
                    VkBuffer& buffer, VkDeviceMemory& memory) const;
  void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size) const;

  // One-shot command-buffer helpers (used for layout transitions / staging copies)
  VkCommandBuffer beginSingleTimeCommands() const;
  void            endSingleTimeCommands(VkCommandBuffer cb) const;

  // Image / texture helpers
  void transitionImageLayout(VkImage image, VkFormat format,
                             VkImageLayout oldLayout, VkImageLayout newLayout) const;
  void copyBufferToImage(VkBuffer buffer, VkImage image,
                         uint32_t width, uint32_t height) const;

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
//  Framebuffers                                                               //
// -------------------------------------------------------------------------- //
// One framebuffer per swapchain image. Each binds [color view, depth view] in
// the same order as the render pass attachments.
void Vulkan42::Impl::createFramebuffers() {
  swapchainFramebuffers.resize(swapchainImageViews.size());

  for (size_t i = 0; i < swapchainImageViews.size(); ++i) {
    VkImageView attachments[] = {swapchainImageViews[i], depthImageView};

    VkFramebufferCreateInfo fbInfo{};
    fbInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbInfo.renderPass      = renderPass;
    fbInfo.attachmentCount = 2;
    fbInfo.pAttachments    = attachments;
    fbInfo.width           = swapchainExtent.width;
    fbInfo.height          = swapchainExtent.height;
    fbInfo.layers          = 1;

    if (vkCreateFramebuffer(device, &fbInfo, nullptr, &swapchainFramebuffers[i]) != VK_SUCCESS)
      throw std::runtime_error("Failed to create framebuffer");
  }
  std::cout << GREEN << "[Vulkan] " << swapchainFramebuffers.size()
            << " framebuffers created" << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Command pool + command buffers                                             //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::createCommandPool() {
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  // RESET_COMMAND_BUFFER_BIT lets us re-record a buffer every frame
  poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilies.graphics;

  if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
    throw std::runtime_error("Failed to create command pool");

  std::cout << GREEN << "[Vulkan] Command pool created" << ENDC << std::endl;
}

void Vulkan42::Impl::createCommandBuffers() {
  commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool        = commandPool;
  allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    throw std::runtime_error("Failed to allocate command buffers");
}

// -------------------------------------------------------------------------- //
//  Synchronization primitives                                                 //
// -------------------------------------------------------------------------- //
// Sync layout: imageAvailable (per-frame) + inFlight (per-frame) created here.
// renderFinished semaphores are sized to swapchain images (see below).
void Vulkan42::Impl::createSyncObjects() {
  imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
  renderFinishedSemaphores.resize(swapchainImages.size());

  VkSemaphoreCreateInfo semInfo{};
  semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  // Start in signaled state so the first frame does not block forever
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    if (vkCreateSemaphore(device, &semInfo,  nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateFence    (device, &fenceInfo, nullptr, &inFlightFences[i])           != VK_SUCCESS)
      throw std::runtime_error("Failed to create per-frame sync objects");
  }
  for (size_t i = 0; i < swapchainImages.size(); ++i) {
    if (vkCreateSemaphore(device, &semInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS)
      throw std::runtime_error("Failed to create per-image renderFinished semaphore");
  }
  std::cout << GREEN << "[Vulkan] Sync objects created ("
            << MAX_FRAMES_IN_FLIGHT << " frames in flight, "
            << swapchainImages.size() << " present semaphores)" << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Uniform buffers (one per frame in flight)                                  //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::createUniformBuffers() {
  const VkDeviceSize bufSize = sizeof(UboMVP);

  uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
  uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
  uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    createBuffer(bufSize,
                 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                 | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 uniformBuffers[i], uniformBuffersMemory[i]);

    // Persistent mapping: we update the UBO every frame from the CPU, so we
    // map it once and write through the pointer each time.
    vkMapMemory(device, uniformBuffersMemory[i], 0, bufSize, 0, &uniformBuffersMapped[i]);
  }
  std::cout << GREEN << "[Vulkan] Uniform buffers created (per-frame)" << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Fallback texture (1x1 white pixel)                                         //
// -------------------------------------------------------------------------- //
// The descriptor set always binds a sampler, even when the user has not
// provided a texture. We upload a 1x1 white texel so the texture branch in the
// shader stays valid; the blend factor (push constant) controls visibility.
void Vulkan42::Impl::createFallbackTexture() {
  uint8_t pixel[4] = {255, 255, 255, 255};
  Texture white(1, 1, std::vector<uint8_t>(pixel, pixel + 4));
  uploadTexture(white);
}

// -------------------------------------------------------------------------- //
//  Descriptor pool + descriptor sets                                          //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::createDescriptorPool() {
  VkDescriptorPoolSize poolSizes[2]{};
  poolSizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSizes[0].descriptorCount = MAX_FRAMES_IN_FLIGHT;
  poolSizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[1].descriptorCount = MAX_FRAMES_IN_FLIGHT;

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = 2;
  poolInfo.pPoolSizes    = poolSizes;
  poolInfo.maxSets       = MAX_FRAMES_IN_FLIGHT;

  if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
    throw std::runtime_error("Failed to create descriptor pool");
}

void Vulkan42::Impl::createDescriptorSets() {
  std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool     = descriptorPool;
  allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
  allocInfo.pSetLayouts        = layouts.data();

  descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
  if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
    throw std::runtime_error("Failed to allocate descriptor sets");

  writeDescriptorSets();
  std::cout << GREEN << "[Vulkan] Descriptor sets allocated and updated" << ENDC << std::endl;
}

// Re-bind the current uniform buffer + texture sampler into existing descriptor
// sets. Called from createDescriptorSets() and from setTexture() when the
// texture image is swapped (no re-allocation -> safe to call repeatedly).
void Vulkan42::Impl::writeDescriptorSets() {
  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    VkDescriptorBufferInfo bufInfo{};
    bufInfo.buffer = uniformBuffers[i];
    bufInfo.offset = 0;
    bufInfo.range  = sizeof(UboMVP);

    VkDescriptorImageInfo imgInfo{};
    imgInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imgInfo.imageView   = textureImageView;
    imgInfo.sampler     = textureSampler;

    VkWriteDescriptorSet writes[2]{};
    writes[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].dstSet          = descriptorSets[i];
    writes[0].dstBinding      = 0;
    writes[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writes[0].descriptorCount = 1;
    writes[0].pBufferInfo     = &bufInfo;

    writes[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[1].dstSet          = descriptorSets[i];
    writes[1].dstBinding      = 1;
    writes[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writes[1].descriptorCount = 1;
    writes[1].pImageInfo      = &imgInfo;

    vkUpdateDescriptorSets(device, 2, writes, 0, nullptr);
  }
}

// -------------------------------------------------------------------------- //
//  Buffer helpers                                                             //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                                   VkMemoryPropertyFlags props,
                                   VkBuffer& buffer, VkDeviceMemory& memory) const
{
  VkBufferCreateInfo bufInfo{};
  bufInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufInfo.size        = size;
  bufInfo.usage       = usage;
  bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(device, &bufInfo, nullptr, &buffer) != VK_SUCCESS)
    throw std::runtime_error("Failed to create buffer");

  VkMemoryRequirements memReqs;
  vkGetBufferMemoryRequirements(device, buffer, &memReqs);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize  = memReqs.size;
  allocInfo.memoryTypeIndex = findMemoryType(memReqs.memoryTypeBits, props);

  if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
    throw std::runtime_error("Failed to allocate buffer memory");

  vkBindBufferMemory(device, buffer, memory, 0);
}

VkCommandBuffer Vulkan42::Impl::beginSingleTimeCommands() const {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool        = commandPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer cb;
  vkAllocateCommandBuffers(device, &allocInfo, &cb);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(cb, &beginInfo);
  return cb;
}

void Vulkan42::Impl::endSingleTimeCommands(VkCommandBuffer cb) const {
  vkEndCommandBuffer(cb);

  VkSubmitInfo submit{};
  submit.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit.commandBufferCount = 1;
  submit.pCommandBuffers    = &cb;

  // Simple wait-idle pattern -- fine for init-time uploads, not for hot path.
  vkQueueSubmit(graphicsQueue, 1, &submit, VK_NULL_HANDLE);
  vkQueueWaitIdle(graphicsQueue);
  vkFreeCommandBuffers(device, commandPool, 1, &cb);
}

void Vulkan42::Impl::copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size) const {
  VkCommandBuffer cb = beginSingleTimeCommands();
  VkBufferCopy region{};
  region.size = size;
  vkCmdCopyBuffer(cb, src, dst, 1, &region);
  endSingleTimeCommands(cb);
}

void Vulkan42::Impl::transitionImageLayout(VkImage image, VkFormat /*format*/,
                                            VkImageLayout oldLayout,
                                            VkImageLayout newLayout) const
{
  VkCommandBuffer cb = beginSingleTimeCommands();

  VkImageMemoryBarrier barrier{};
  barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout                       = oldLayout;
  barrier.newLayout                       = newLayout;
  barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
  barrier.image                           = image;
  barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel   = 0;
  barrier.subresourceRange.levelCount     = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount     = 1;

  VkPipelineStageFlags srcStage, dstStage;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
      newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    srcStage              = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dstStage              = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    srcStage              = VK_PIPELINE_STAGE_TRANSFER_BIT;
    dstStage              = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else {
    throw std::runtime_error("Unsupported image layout transition");
  }

  vkCmdPipelineBarrier(cb, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
  endSingleTimeCommands(cb);
}

void Vulkan42::Impl::copyBufferToImage(VkBuffer buffer, VkImage image,
                                        uint32_t width, uint32_t height) const
{
  VkCommandBuffer cb = beginSingleTimeCommands();

  VkBufferImageCopy region{};
  region.bufferOffset                    = 0;
  region.bufferRowLength                 = 0;
  region.bufferImageHeight               = 0;
  region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel       = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount     = 1;
  region.imageOffset                     = {0, 0, 0};
  region.imageExtent                     = {width, height, 1};

  vkCmdCopyBufferToImage(cb, buffer, image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
  endSingleTimeCommands(cb);
}

// -------------------------------------------------------------------------- //
//  Mesh upload (vertex + index buffers via staging)                           //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::uploadMesh(const Mesh& m) {
  destroyMeshBuffers(); // safe even if not allocated

  const auto& verts = m.vertices();
  const auto& idx   = m.indices();
  if (verts.empty() || idx.empty()) {
    meshUploaded = false;
    return;
  }

  // --- Vertex buffer ---
  {
    const VkDeviceSize size = sizeof(Vertex) * verts.size();
    VkBuffer       staging;
    VkDeviceMemory stagingMem;
    createBuffer(size,
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                 | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 staging, stagingMem);

    void* data;
    vkMapMemory(device, stagingMem, 0, size, 0, &data);
    std::memcpy(data, verts.data(), static_cast<size_t>(size));
    vkUnmapMemory(device, stagingMem);

    createBuffer(size,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 vertexBuffer, vertexBufferMemory);
    copyBuffer(staging, vertexBuffer, size);

    vkDestroyBuffer(device, staging, nullptr);
    vkFreeMemory(device, stagingMem, nullptr);
  }

  // --- Index buffer ---
  {
    const VkDeviceSize size = sizeof(uint32_t) * idx.size();
    VkBuffer       staging;
    VkDeviceMemory stagingMem;
    createBuffer(size,
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                 | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 staging, stagingMem);

    void* data;
    vkMapMemory(device, stagingMem, 0, size, 0, &data);
    std::memcpy(data, idx.data(), static_cast<size_t>(size));
    vkUnmapMemory(device, stagingMem);

    createBuffer(size,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 indexBuffer, indexBufferMemory);
    copyBuffer(staging, indexBuffer, size);

    vkDestroyBuffer(device, staging, nullptr);
    vkFreeMemory(device, stagingMem, nullptr);
  }

  indexCount   = static_cast<uint32_t>(idx.size());
  meshUploaded = true;
  std::cout << GREEN << "[Vulkan] Mesh uploaded ("
            << verts.size() << " verts, " << idx.size() << " indices)" << ENDC << std::endl;
}

// -------------------------------------------------------------------------- //
//  Texture upload (image + view + sampler)                                    //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::uploadTexture(const Texture& tex) {
  destroyTexture();

  const uint32_t w = tex.width();
  const uint32_t h = tex.height();
  const VkDeviceSize size = static_cast<VkDeviceSize>(w) * h * 4;

  // 1. Staging buffer
  VkBuffer       staging;
  VkDeviceMemory stagingMem;
  createBuffer(size,
               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
               | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               staging, stagingMem);

  void* data;
  vkMapMemory(device, stagingMem, 0, size, 0, &data);
  std::memcpy(data, tex.pixels().data(), static_cast<size_t>(size));
  vkUnmapMemory(device, stagingMem);

  // 2. Device-local image
  createImage(w, h, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
              VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
              textureImage, textureImageMemory);

  // 3. Layout transition -> transfer dst, copy, transition -> shader read
  transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                        VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  copyBufferToImage(staging, textureImage, w, h);
  transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  vkDestroyBuffer(device, staging, nullptr);
  vkFreeMemory(device, stagingMem, nullptr);

  // 4. View + sampler
  textureImageView = createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                                     VK_IMAGE_ASPECT_COLOR_BIT);

  VkSamplerCreateInfo samplerInfo{};
  samplerInfo.sType         = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter     = VK_FILTER_LINEAR;
  samplerInfo.minFilter     = VK_FILTER_LINEAR;
  samplerInfo.addressModeU  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  // Anisotropy disabled -- it would require enabling samplerAnisotropy as a
  // device feature, but the mandatory part doesn't need it.
  samplerInfo.anisotropyEnable        = VK_FALSE;
  samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.compareEnable           = VK_FALSE;
  samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;

  if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS)
    throw std::runtime_error("Failed to create texture sampler");

  std::cout << GREEN << "[Vulkan] Texture uploaded (" << w << "x" << h << ")" << ENDC << std::endl;
}

void Vulkan42::Impl::destroyTexture() {
  if (textureSampler != VK_NULL_HANDLE)   { vkDestroySampler(device, textureSampler, nullptr);   textureSampler   = VK_NULL_HANDLE; }
  if (textureImageView != VK_NULL_HANDLE) { vkDestroyImageView(device, textureImageView, nullptr); textureImageView = VK_NULL_HANDLE; }
  if (textureImage != VK_NULL_HANDLE)     { vkDestroyImage(device, textureImage, nullptr);       textureImage     = VK_NULL_HANDLE; }
  if (textureImageMemory != VK_NULL_HANDLE){vkFreeMemory(device, textureImageMemory, nullptr);   textureImageMemory = VK_NULL_HANDLE; }
}

void Vulkan42::Impl::destroyMeshBuffers() {
  if (vertexBuffer != VK_NULL_HANDLE)      { vkDestroyBuffer(device, vertexBuffer, nullptr);     vertexBuffer       = VK_NULL_HANDLE; }
  if (vertexBufferMemory != VK_NULL_HANDLE){ vkFreeMemory(device, vertexBufferMemory, nullptr);  vertexBufferMemory = VK_NULL_HANDLE; }
  if (indexBuffer != VK_NULL_HANDLE)       { vkDestroyBuffer(device, indexBuffer, nullptr);      indexBuffer        = VK_NULL_HANDLE; }
  if (indexBufferMemory != VK_NULL_HANDLE) { vkFreeMemory(device, indexBufferMemory, nullptr);   indexBufferMemory  = VK_NULL_HANDLE; }
  indexCount   = 0;
  meshUploaded = false;
}

// -------------------------------------------------------------------------- //
//  Per-frame: update UBO, record command buffer                               //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::updateUniformBuffer(uint32_t frame, const FrameState& state) {
  UboMVP ubo{};
  ubo.proj  = state.camera.proj;
  ubo.view  = state.camera.view;
  ubo.model = state.model;
  std::memcpy(uniformBuffersMapped[frame], &ubo, sizeof(ubo));
}

void Vulkan42::Impl::recordCommandBuffer(VkCommandBuffer cb, uint32_t imageIndex,
                                          float blendFactor)
{
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(cb, &beginInfo) != VK_SUCCESS)
    throw std::runtime_error("Failed to begin command buffer");

  // Clear values: index must match render pass attachment order (color, depth)
  VkClearValue clearValues[2]{};
  clearValues[0].color        = {{0.05f, 0.05f, 0.07f, 1.f}};
  clearValues[1].depthStencil = {1.f, 0};

  VkRenderPassBeginInfo rpBegin{};
  rpBegin.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  rpBegin.renderPass        = renderPass;
  rpBegin.framebuffer       = swapchainFramebuffers[imageIndex];
  rpBegin.renderArea.offset = {0, 0};
  rpBegin.renderArea.extent = swapchainExtent;
  rpBegin.clearValueCount   = 2;
  rpBegin.pClearValues      = clearValues;

  vkCmdBeginRenderPass(cb, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

  // Dynamic viewport / scissor (declared in pipeline as dynamic states)
  VkViewport viewport{};
  viewport.x        = 0.f;
  viewport.y        = 0.f;
  viewport.width    = static_cast<float>(swapchainExtent.width);
  viewport.height   = static_cast<float>(swapchainExtent.height);
  viewport.minDepth = 0.f;
  viewport.maxDepth = 1.f;
  vkCmdSetViewport(cb, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapchainExtent;
  vkCmdSetScissor(cb, 0, 1, &scissor);

  if (meshUploaded) {
    VkBuffer     vbs[]    = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(cb, 0, 1, vbs, offsets);
    vkCmdBindIndexBuffer(cb, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                            1, &descriptorSets[currentFrame], 0, nullptr);

    // Push constants (blend factor) -- cheap per-draw uniform
    PushConstants pc{};
    pc.blendFactor = blendFactor;
    vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(PushConstants), &pc);

    vkCmdDrawIndexed(cb, indexCount, 1, 0, 0, 0);
  }

  vkCmdEndRenderPass(cb);

  if (vkEndCommandBuffer(cb) != VK_SUCCESS)
    throw std::runtime_error("Failed to end command buffer");
}

// -------------------------------------------------------------------------- //
//  Swapchain recreation (resize / VK_ERROR_OUT_OF_DATE_KHR)                   //
// -------------------------------------------------------------------------- //
void Vulkan42::Impl::recreateSwapchain() {
  // Handle minimization: wait until we have a non-zero framebuffer
  int w = 0, hh = 0;
  glfwGetFramebufferSize(window->glfw_window, &w, &hh);
  while (w == 0 || hh == 0) {
    glfwGetFramebufferSize(window->glfw_window, &w, &hh);
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(device);
  cleanupSwapchain();

  createSwapchain();
  createImageViews();
  createDepthResources();
  createFramebuffers();

  // Per-image present semaphores were destroyed with the swapchain.
  VkSemaphoreCreateInfo semInfo{};
  semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  renderFinishedSemaphores.resize(swapchainImages.size());
  for (size_t i = 0; i < swapchainImages.size(); ++i) {
    if (vkCreateSemaphore(device, &semInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS)
      throw std::runtime_error("Failed to recreate renderFinished semaphore");
  }
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
  // Per-image present semaphores are tied to swapchainImages -- destroy them
  // with the swapchain so the recreation path can resize them cleanly.
  for (auto s : renderFinishedSemaphores)
    vkDestroySemaphore(device, s, nullptr);
  renderFinishedSemaphores.clear();

  // Framebuffers reference both the swapchain views and the depth view, so
  // they must go first.
  for (auto fb : swapchainFramebuffers)
    vkDestroyFramebuffer(device, fb, nullptr);
  swapchainFramebuffers.clear();

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
  // Destroy in reverse creation order. waitIdle() is the caller's
  // responsibility, but we are defensive here too.
  if (device != VK_NULL_HANDLE) vkDeviceWaitIdle(device);

  cleanupSwapchain();

  destroyTexture();
  destroyMeshBuffers();

  for (size_t i = 0; i < uniformBuffers.size(); ++i) {
    if (uniformBuffersMapped[i]) vkUnmapMemory(device, uniformBuffersMemory[i]);
    vkDestroyBuffer(device, uniformBuffers[i], nullptr);
    vkFreeMemory   (device, uniformBuffersMemory[i], nullptr);
  }
  uniformBuffers.clear();
  uniformBuffersMemory.clear();
  uniformBuffersMapped.clear();

  if (descriptorPool != VK_NULL_HANDLE) {
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    descriptorPool = VK_NULL_HANDLE;
  }

  for (auto s : imageAvailableSemaphores) vkDestroySemaphore(device, s, nullptr);
  for (auto s : renderFinishedSemaphores) vkDestroySemaphore(device, s, nullptr);
  for (auto f : inFlightFences)           vkDestroyFence    (device, f, nullptr);
  imageAvailableSemaphores.clear();
  renderFinishedSemaphores.clear();
  inFlightFences.clear();

  if (commandPool != VK_NULL_HANDLE) {
    // freeing the pool frees all its command buffers
    vkDestroyCommandPool(device, commandPool, nullptr);
    commandPool = VK_NULL_HANDLE;
    commandBuffers.clear();
  }

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
  impl->createFramebuffers();
  impl->createCommandPool();

  // Default texture before descriptor sets so the binding has a real sampler/view
  impl->createFallbackTexture();

  impl->createUniformBuffers();
  impl->createDescriptorPool();
  impl->createDescriptorSets();
  impl->createCommandBuffers();
  impl->createSyncObjects();

  impl->inited = true;
  std::cout << GREEN << "[Vulkan] Initialized " << impl->width << "x" << impl->height
            << ENDC << std::endl;
}

void Vulkan42::resize(int width, int height) {
  impl->width  = width;
  impl->height = height;
  // We don't recreate immediately; setting the flag defers the work to the next
  // draw(). This keeps Vulkan calls off the GLFW callback thread.
  impl->framebufferResized = true;
}

void Vulkan42::setMesh(const Mesh& mesh) {
  impl->mesh = mesh;
  if (!impl->inited) return;
  // Avoid destroying buffers that might still be referenced by an in-flight frame
  vkDeviceWaitIdle(impl->device);
  impl->uploadMesh(mesh);
}

void Vulkan42::setTexture(const Texture* texture) {
  impl->texture = texture;
  if (!impl->inited) return;
  vkDeviceWaitIdle(impl->device);

  if (texture != nullptr && !texture->pixels().empty())
    impl->uploadTexture(*texture);
  else
    impl->createFallbackTexture();

  // The texture image+view+sampler were rebuilt -- rebinding (not re-allocating)
  // the existing descriptor sets is enough.
  if (!impl->descriptorSets.empty())
    impl->writeDescriptorSets();
}

void Vulkan42::setRenderMode(RenderMode mode) {
  impl->mode = mode;
}

void Vulkan42::draw(const FrameState& state) {
  if (!impl->inited) return;

  // 1. Wait for the previous use of this frame slot to finish
  vkWaitForFences(impl->device, 1, &impl->inFlightFences[impl->currentFrame],
                  VK_TRUE, UINT64_MAX);

  // 2. Acquire next swapchain image
  uint32_t imageIndex;
  VkResult acquireRes = vkAcquireNextImageKHR(
      impl->device, impl->swapchain, UINT64_MAX,
      impl->imageAvailableSemaphores[impl->currentFrame],
      VK_NULL_HANDLE, &imageIndex);

  if (acquireRes == VK_ERROR_OUT_OF_DATE_KHR) {
    impl->recreateSwapchain();
    return; // skip this frame
  } else if (acquireRes != VK_SUCCESS && acquireRes != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("Failed to acquire swapchain image");
  }

  // Only reset the fence once we know we're submitting -- otherwise we'd
  // deadlock if we returned early above.
  vkResetFences(impl->device, 1, &impl->inFlightFences[impl->currentFrame]);

  // 3. Update per-frame data
  impl->updateUniformBuffer(impl->currentFrame, state);

  // 4. Re-record this frame's command buffer
  VkCommandBuffer cb = impl->commandBuffers[impl->currentFrame];
  vkResetCommandBuffer(cb, 0);
  impl->recordCommandBuffer(cb, imageIndex, state.blendFactor);

  // 5. Submit
  VkSemaphore          waitSems[]   = {impl->imageAvailableSemaphores[impl->currentFrame]};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  // renderFinished is per swapchain image (not per frame-in-flight) so the
  // present queue is never asked to reuse a still-pending semaphore.
  VkSemaphore          signalSems[] = {impl->renderFinishedSemaphores[imageIndex]};

  VkSubmitInfo submitInfo{};
  submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.waitSemaphoreCount   = 1;
  submitInfo.pWaitSemaphores      = waitSems;
  submitInfo.pWaitDstStageMask    = waitStages;
  submitInfo.commandBufferCount   = 1;
  submitInfo.pCommandBuffers      = &cb;
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores    = signalSems;

  if (vkQueueSubmit(impl->graphicsQueue, 1, &submitInfo,
                    impl->inFlightFences[impl->currentFrame]) != VK_SUCCESS)
    throw std::runtime_error("Failed to submit draw command buffer");

  // 6. Present
  VkPresentInfoKHR presentInfo{};
  presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores    = signalSems;
  presentInfo.swapchainCount     = 1;
  presentInfo.pSwapchains        = &impl->swapchain;
  presentInfo.pImageIndices      = &imageIndex;

  VkResult presentRes = vkQueuePresentKHR(impl->presentQueue, &presentInfo);

  if (presentRes == VK_ERROR_OUT_OF_DATE_KHR ||
      presentRes == VK_SUBOPTIMAL_KHR        ||
      impl->framebufferResized)
  {
    impl->framebufferResized = false;
    impl->recreateSwapchain();
  } else if (presentRes != VK_SUCCESS) {
    throw std::runtime_error("Failed to present swapchain image");
  }

  impl->currentFrame = (impl->currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
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
