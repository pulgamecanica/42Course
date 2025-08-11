#ifndef SCOP_OBJLOADER_HPP
#define SCOP_OBJLOADER_HPP

#include <string>
#include "Mesh.hpp"

namespace scop {
  
class ObjLoader {
public:
  // computeNormalsIfMissing: generate vertex normals if the OBJ lacks them
  // triangulate: fan-triangulate faces (good enough for mandatory; refine later)
  Mesh load(const std::string& path,
            bool computeNormalsIfMissing = true,
            bool triangulate = true) const;
};

} // namespace scop

#endif // SCOP_OBJLOADER_HPP
