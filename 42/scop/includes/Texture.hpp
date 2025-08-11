#ifndef SCOP_TEXTURE_HPP
#define SCOP_TEXTURE_HPP

#include <cstdint>
#include <vector>
#include <string>

namespace scop {

class Texture {
public:
  Texture() = default;
  Texture(uint32_t w, uint32_t h, std::vector<uint8_t> rgba);

  uint32_t width()  const;
  uint32_t height() const;
  const std::vector<uint8_t>& pixels() const; // RGBA8

  // Helper for PNG-with-lodepng path (optional to implement later)
  static Texture fromPNG(const std::string& filePath);

private:
  uint32_t _w = 0, _h = 0;
  std::vector<uint8_t> _rgba;
};

} // namespace scop

#endif // SCOP_TEXTURE_HPP
