#ifndef SCOP_VULKAN42_HPP
#define SCOP_VULKAN42_HPP

#include "IRenderer.hpp"

namespace scop {
class Vulkan42 final : public IRenderer {
public:
	Vulkan42();
	~Vulkan42() override;

	void init(Window& window) override;
	void resize(int width, int height) override;
	void setMesh(const Mesh& mesh) override;
	void setTexture(const Texture* texture) override;
	void setRenderMode(RenderMode mode) override;
	void draw(const FrameState& state) override;
	void waitIdle() override;

private:
	// pimpl-friendly: keep raw pointers/handles out of the header if you want
	struct Impl;
	Impl* impl; // allocated in ctor, freed in dtor
};

std::ostream&	operator<<(std::ostream&, const Vulkan42&);

} // namespace scop

#endif // SCOP_VULKAN42_HPP
