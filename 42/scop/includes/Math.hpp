#ifndef SCOP_MATH_HPP
#define SCOP_MATH_HPP

#include <cstdint>

namespace scop {
  
struct Vec2 { float x, y; };
struct Vec3 { float x, y, z; };

struct Mat4 {
  float m[16]; // matrix 3d (projection matrix compatible)

  static Mat4 identity();
  static Mat4 perspective(float fovyRadians, float aspect, float znear, float zfar);
  static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);

  static Mat4 translate(const Vec3& t);
  static Mat4 scale(const Vec3& s);
  static Mat4 rotateAxisAngle(const Vec3& axis, float radians);

  Mat4 operator*(const Mat4& rhs) const;
};

} // namespace scop

#endif // SCOP_MATH_HPP
