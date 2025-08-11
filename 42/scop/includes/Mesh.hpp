#ifndef SCOP_MESH_HPP
#define SCOP_MESH_HPP

#include <cstdint>
#include <vector>
#include "Math.hpp"

namespace scop {

struct Vertex {
  Vec3     pos;
  Vec3     normal;
  Vec2     uv;
  uint32_t faceId; // for per-face coloring
};

class Mesh {
public:
  Mesh() = default;
  Mesh(std::vector<Vertex> v, std::vector<uint32_t> i, const Vec3& centroid, const Vec3& mainAxis);

  const std::vector<Vertex>&   vertices() const;
  const std::vector<uint32_t>& indices() const;

  const Vec3& centroid() const;   // used to rotate about center
  const Vec3& mainAxis() const;   // default auto-rotation axis (e.g., Y)

private:
  std::vector<Vertex>   _vertices;
  std::vector<uint32_t> _indices;
  Vec3 _centroid {0,0,0};
  Vec3 _mainAxis {0,1,0};
};

} // namespace scop

#endif // SCOP_MESH_HPP
