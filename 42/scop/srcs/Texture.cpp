#include "Texture.hpp"
#include "lodepng.h"
#include <stdexcept>
#include <cstdlib>

namespace scop {

Texture::Texture(uint32_t w, uint32_t h, std::vector<uint8_t> rgba)
: _w(w), _h(h), _rgba(std::move(rgba)) {}

uint32_t Texture::width() const { return _w; }
uint32_t Texture::height() const { return _h; }
const std::vector<uint8_t>& Texture::pixels() const { return _rgba; }

Texture Texture::fromPNG(const std::string& filePath) {
  unsigned char* out = nullptr;
  unsigned w=0, h=0;
  const unsigned err = lodepng_decode32_file(&out, &w, &h, filePath.c_str());
  if (err != 0) {
    throw std::runtime_error(lodepng_error_text(err));
  }
  std::vector<uint8_t> data(out, out + (w * h * 4));
  free(out);
  return Texture(static_cast<uint32_t>(w), static_cast<uint32_t>(h), std::move(data));
}

} // namespace scop
