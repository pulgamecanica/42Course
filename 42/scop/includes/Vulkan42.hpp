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
#include <limits>
#include <algorithm>


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
  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };
  class Vulkan42 {
    static const int MAX_FRAMES_IN_FLIGHT;
    public:
      Vulkan42(const Window & win);
      ~Vulkan42();
      void                    drawFrame();
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
      void                    createCommandBuffers();
      void                    createSyncObjects();
      void                    recreateSwapChain();
      void                    cleanupSwapChain();
      void                    pickFirstSuitablePhysicalDevice();
      bool                    isPhysicalDeviceSuitable(VkPhysicalDevice device);
      bool                    checkDeviceExtensionSupport(VkPhysicalDevice device);
      VkShaderModule          createShaderModule(const std::vector<char>& code);
      QueueFamInd             findQueueFamilies(VkPhysicalDevice device);
      SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, bool debug);
      VkSurfaceFormatKHR      chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
      VkPresentModeKHR        chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
      VkExtent2D              chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
      void                    recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);


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
      const Window  &                win;
  };
  std::ostream& operator<<(std::ostream&, const Vulkan42&);
}
#endif

