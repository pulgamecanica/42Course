//***************************//
//*Template by pulgamecanica*//
//***************************//
#ifndef __SCOP_HPP__
# define __SCOP_HPP__

# define BLACK  "\033[0;30m"
# define RED    "\033[0;31m"
# define GREEN  "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE   "\033[0;34m"
# define WHITE  "\033[0;37m"
# define ENDC   "\033[0m"

# define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Window.hpp"
#include <iostream>
#include <sys/time.h>
#include <vector>

namespace scop {
  typedef VkPhysicalDevice VKDevice;
  typedef enum ScopStatus {
    Menu,
    Settings,
    Rendering 
  } ScopStatus;
  class Scop {
    public:
      Scop();
      ~Scop();
      ScopStatus  getStatus() const;
      bool        checkStatus(ScopStatus status) const;
      void        runScop();
      void        keyEvent(int key, int action, int mods);
      void        mouseButtonEvent(int key, int action, int mods);
      void        mouseMoveEvent(double xpos, double ypos);
      void        resize(int width, int height);
    private:
      void        initVulkan();
      void        createVkInstance();
      void        pickPhysicalDevice();
      bool        isDeviceSuitable(VKDevice device);

      ScopStatus  status;
      VkInstance  instance;
      VKDevice    physicalDevice;
      Window  *   win;
      uint64_t    created_at;
      uint64_t    updated_at;
  };
  std::ostream& operator<<(std::ostream&, const Scop&);
}

#endif

