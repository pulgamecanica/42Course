#ifndef SCOP_VULKAN42_HPP
#define SCOP_VULKAN42_HPP

#include "IRenderer.hpp"
#include <iostream>

namespace scop {

class Vulkan42 final : public IRenderer {
public:
  Vulkan42();
  ~Vulkan42() override;

  Vulkan42(const Vulkan42&) = delete;
  Vulkan42& operator=(const Vulkan42&) = delete;

  void init(Window& window) override;
  void resize(int width, int height) override;
  void setMesh(const Mesh& mesh) override;
  void setTexture(const Texture* texture) override;
  void setRenderMode(RenderMode mode) override;
  void draw(const FrameState& state) override;
  void waitIdle() override;

private:
  struct Impl;
  Impl* impl;
};

std::ostream& operator<<(std::ostream&, const Vulkan42&);

} // namespace scop

#endif // SCOP_VULKAN42_HPP
