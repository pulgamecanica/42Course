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
// #include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <GLFW/glfw3.h>
#include "colors.hpp"
#include "Window.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <optional>
#include <limits>
#include <algorithm>
#include <glm/glm.hpp>


namespace scop {
  struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;
  };
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
  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };
  class Vulkan42 {
    static const int MAX_FRAMES_IN_FLIGHT;
    public:
      Vulkan42(Window * & win);
      ~Vulkan42();
      void                        drawFrame();
      VkInstance &                getInstance();
      VkDevice &                  getDevice();
      ImGui_ImplVulkan_InitInfo   getImGui_ImplVulkan_InitInfo() const;
      ImGui_ImplVulkanH_Window *  getImGui_ImplVulkanH_Window() const;
    private:
      void                    createVkInstance(const char * application_name);
      void                    createSwapChain();
      void                    createSurface();
      void                    createLogicalDevice();
      void                    createImageViews();
      void                    createGraphicsPipeline();
      void                    createRenderPass();
      void                    createFramebuffers();
      void                    createCommandPool();
      void                    createVertexBuffer();
      void                    createCommandBuffers();
      void                    createSyncObjects();
      void                    createBindingDescriptionAndAttributeDescriptions();
      void                    recreateSwapChain();
      void                    cleanupSwapChain();
      void                    pickFirstSuitablePhysicalDevice();
      bool                    isPhysicalDeviceSuitable(VkPhysicalDevice device);
      bool                    checkDeviceExtensionSupport(VkPhysicalDevice device);
      VkShaderModule          createShaderModule(const std::vector<char>& code);
      QueueFamInd             findQueueFamilies(VkPhysicalDevice device) const;
      SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, bool debug) const;
      VkSurfaceFormatKHR      chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
      VkPresentModeKHR        chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
      VkExtent2D              chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
      void                    recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
      uint32_t                findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

      uint32_t                      currentFrame;
      VkInstance                    instance;
      VkSurfaceKHR                  surface;
      VkSwapchainKHR                swapChain;
      VkExtent2D                    swapChainExtent;
      VkFormat                      swapChainImageFormat;
      VkRenderPass                  renderPass;
      VkPipelineLayout              pipelineLayout;
      VkPipeline                    graphicsPipeline;
      VkCommandBuffer               commandBuffer;
      VkCommandPool                 commandPool;
      VkBuffer                      vertexBuffer;

      VkPipelineCache               pipelineCache;
      VkDescriptorPool              descriptorPool;

      VkVertexInputBindingDescription bindingDescription;
      std::vector<VkVertexInputAttributeDescription> bindingAttributeDescriptions;

      std::vector<VkImage>          swapChainImages;
      std::vector<VkImageView>      swapChainImageViews;
      std::vector<VkFramebuffer>    swapChainFramebuffers;
      std::vector<VkCommandBuffer>  commandBuffers;
      std::vector<VkSemaphore>      imageAvailableSemaphores;
      std::vector<VkSemaphore>      renderFinishedSemaphores;
      std::vector<VkFence>          inFlightFences;
      /* 
       * A physical device in this case could be a Graphics Card for example
       * In my case, the device selected is: AMD Radeon Vega 3 Graphics (RADV RAVEN2)
       */
      VkPhysicalDevice               physicalDevice;
      VkDevice                       device;
      VkQueue                        graphicsQueue;
      VkQueue                        presentQueue;
      Window * &               win;
  };
  std::ostream& operator<<(std::ostream&, const Vulkan42&);
}
#endif

