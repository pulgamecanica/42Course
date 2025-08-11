#include "Vulkan42.hpp"
#include "Window.hpp"
#include "Mesh.hpp"
#include <iostream>

namespace scop {

struct Vulkan42::Impl {
  Window* window = nullptr;
  int width = 0, height = 0;
  RenderMode mode = RenderMode::Faces;
  Mesh mesh;
  const Texture* texture = nullptr;
  bool inited = false;
};

Vulkan42::Vulkan42() : impl(new Impl) {}
Vulkan42::~Vulkan42() { delete impl; }

void Vulkan42::init(Window& window) {
  impl->window = &window;
  impl->width  = window.getWidth();
  impl->height = window.getHeight();
  impl->inited = true;

  // NOTE: this is a stub. You'll create VkInstance/device/swapchain/pipeline here.
  std::cout << "[Vulkan42] init " << impl->width << "x" << impl->height << std::endl;
}

void Vulkan42::resize(int width, int height) {
  impl->width = width; impl->height = height;
  // Stub: later recreate swapchain & framebuffers
  std::cout << "[Vulkan42] resize -> " << width << "x" << height << std::endl;
}

void Vulkan42::setMesh(const Mesh& mesh) {
  impl->mesh = mesh; // copy; later you’ll upload to GPU buffers
}

void Vulkan42::setTexture(const Texture* texture) {
  impl->texture = texture; // store pointer; later create VkImage+sampler
}

void Vulkan42::setRenderMode(RenderMode mode) {
  impl->mode = mode;
}

void Vulkan42::draw(const FrameState& /*state*/) {
  if (!impl->inited) return;
  // Stub: do nothing. Later: update UBOs, record cmd buffer, submit, present.
}

void Vulkan42::waitIdle() {
  // Stub: vkDeviceWaitIdle
}

std::ostream& operator<<(std::ostream& s, const Vulkan42& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

} // namespace scop
