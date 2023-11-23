//***************************//
//*Template by pulgamecanica*//
//***************************//
#ifndef __SCOP_HPP__
# define __SCOP_HPP__

#ifndef SCOP_DEBUG
# define SCOP_DEBUG 1
#endif

# define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "colors.hpp"
#include "Window.hpp"
#include "Vulkan42.hpp"
#include <iostream>
#include <sys/time.h>

namespace scop {
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
      ScopStatus  status;
      Vulkan42  *       vulkan;
      Window  *         win;
      uint64_t          created_at;
      uint64_t          updated_at;
  };
  std::ostream& operator<<(std::ostream&, const Scop&);
}
#endif
