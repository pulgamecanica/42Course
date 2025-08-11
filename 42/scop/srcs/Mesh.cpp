#include "Mesh.hpp"

namespace scop {

Mesh::Mesh(std::vector<Vertex> v,
           std::vector<uint32_t> i,
           const Vec3& centroid,
           const Vec3& mainAxis)
: _vertices(std::move(v))
, _indices(std::move(i))
, _centroid(centroid)
, _mainAxis(mainAxis) {}

const std::vector<Vertex>&   Mesh::vertices() const { return _vertices; }
const std::vector<uint32_t>& Mesh::indices()  const { return _indices; }
const Vec3& Mesh::centroid() const { return _centroid; }
const Vec3& Mesh::mainAxis() const { return _mainAxis; }

} // namespace scop
