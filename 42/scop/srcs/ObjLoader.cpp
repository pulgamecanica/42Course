#include "ObjLoader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <cctype>
#include <cmath>

namespace scop {

struct Idx { int v=-1, vt=-1, vn=-1; };

// Parse a token like "v", "v/t", "v//n", "v/t/n" into Idx (1-based -> 0-based)
static inline Idx parseFaceElem(const std::string& tok) {
  Idx out;
  int slash1 = tok.find('/');
  if (slash1 == (int)std::string::npos) { // "v"
    out.v = std::stoi(tok) - 1;
    return out;
  }
  int slash2 = tok.find('/', slash1 + 1);
  out.v = std::stoi(tok.substr(0, slash1)) - 1;
  if (slash2 == (int)std::string::npos) { // "v/t"
    out.vt = std::stoi(tok.substr(slash1+1)) - 1;
  } else if (slash2 == slash1 + 1) { // "v//n"
    out.vn = std::stoi(tok.substr(slash2+1)) - 1;
  } else { // "v/t/n"
    out.vt = std::stoi(tok.substr(slash1+1, slash2 - (slash1+1))) - 1;
    out.vn = std::stoi(tok.substr(slash2+1)) - 1;
  }
  return out;
}

static inline Vec3 vec3_add(const Vec3& a, const Vec3& b){ return {a.x+b.x, a.y+b.y, a.z+b.z}; }
static inline Vec3 vec3_sub(const Vec3& a, const Vec3& b){ return {a.x-b.x, a.y-b.y, a.z-b.z}; }
static inline Vec3 vec3_cross(const Vec3& a, const Vec3& b){
  return { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
}
static inline float vec3_len(const Vec3& v){ return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z); }
static inline Vec3 vec3_norm(const Vec3& v){
  float l = vec3_len(v); return (l>0.f)? Vec3{v.x/l, v.y/l, v.z/l} : Vec3{0,0,0};
}

Mesh ObjLoader::load(const std::string& path,
                     bool computeNormalsIfMissing,
                     bool triangulate) const
{
  std::ifstream in(path);
  if (!in) throw std::runtime_error("Cannot open OBJ: " + path);

  std::vector<Vec3> positions;
  std::vector<Vec3> normals;
  std::vector<Vec2> uvs;

  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  std::string line;
  uint32_t faceId = 0;

  while (std::getline(in, line)) {
    if (line.empty() || line[0] == '#') continue;
    std::istringstream ss(line);
    std::string tag; ss >> tag;
    if (tag == "v") {
      Vec3 p{}; ss >> p.x >> p.y >> p.z; positions.push_back(p);
    } else if (tag == "vt") {
      Vec2 t{}; ss >> t.x >> t.y; uvs.push_back(t);
    } else if (tag == "vn") {
      Vec3 n{}; ss >> n.x >> n.y >> n.z; normals.push_back(n);
    } else if (tag == "f") {
      // collect all tokens in the face
      std::vector<Idx> fidx;
      std::string tok;
      while (ss >> tok) fidx.push_back(parseFaceElem(tok));
      if (fidx.size() < 3) continue;

      // triangulate as fan: (0,i-1,i)
      const size_t triCount = triangulate ? (fidx.size()-2) : 1;
      for (size_t i = 0; i < triCount; ++i) {
        Idx i0 = fidx[0], i1 = fidx[i+1], i2 = fidx[i+2];

        // Gather P/N/UV with bounds checks
        auto safeP = [&](int idx)->Vec3 { return (idx>=0 && (size_t)idx<positions.size()) ? positions[idx] : Vec3{0,0,0}; };
        auto safeN = [&](int idx)->Vec3 { return (idx>=0 && (size_t)idx<normals.size())  ? normals[idx]  : Vec3{0,0,1}; };
        auto safeT = [&](int idx)->Vec2 { return (idx>=0 && (size_t)idx<uvs.size())      ? uvs[idx]      : Vec2{0,0}; };

        Vec3 p0 = safeP(i0.v), p1 = safeP(i1.v), p2 = safeP(i2.v);
        Vec3 n0, n1, n2;

        if (i0.vn>=0 && i1.vn>=0 && i2.vn>=0) {
          n0 = safeN(i0.vn); n1 = safeN(i1.vn); n2 = safeN(i2.vn);
        } else if (computeNormalsIfMissing) {
          Vec3 fn = vec3_norm(vec3_cross( vec3_sub(p1,p0), vec3_sub(p2,p0)));
          n0 = n1 = n2 = fn;
        } else {
          n0 = n1 = n2 = {0,0,1};
        }

        Vec2 t0 = safeT(i0.vt), t1 = safeT(i1.vt), t2 = safeT(i2.vt);

        const uint32_t base = static_cast<uint32_t>(vertices.size());
        vertices.push_back( Vertex{ p0, n0, t0, faceId } );
        vertices.push_back( Vertex{ p1, n1, t1, faceId } );
        vertices.push_back( Vertex{ p2, n2, t2, faceId } );
        indices.push_back(base+0);
        indices.push_back(base+1);
        indices.push_back(base+2);

        ++faceId;
      }
    }
  }

  // Compute centroid
  Vec3 c{0,0,0};
  if (!vertices.empty()) {
    for (const auto& v : vertices) c = vec3_add(c, v.pos);
    c = { c.x / float(vertices.size()), c.y / float(vertices.size()), c.z / float(vertices.size()) };
  }

  // Heuristic main axis: choose largest extent axis
  Vec3 minB{+1e9f, +1e9f, +1e9f}, maxB{-1e9f,-1e9f,-1e9f};
  for (const auto& v : vertices) {
    minB.x = std::min(minB.x, v.pos.x); minB.y = std::min(minB.y, v.pos.y); minB.z = std::min(minB.z, v.pos.z);
    maxB.x = std::max(maxB.x, v.pos.x); maxB.y = std::max(maxB.y, v.pos.y); maxB.z = std::max(maxB.z, v.pos.z);
  }
  Vec3 size{ maxB.x-minB.x, maxB.y-minB.y, maxB.z-minB.z };
  Vec3 axis{0,1,0};
  if (size.x >= size.y && size.x >= size.z) axis = {1,0,0};
  else if (size.z >= size.x && size.z >= size.y) axis = {0,0,1};

  return Mesh(std::move(vertices), std::move(indices), c, axis);
}

} // namespace scop
