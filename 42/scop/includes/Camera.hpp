#ifndef SCOP_CAMERA_HPP
#define SCOP_CAMERA_HPP

#include "Math.hpp"

namespace scop {

class Camera {
public:
  Camera();

  void setPerspective(float fovRadians, float aspect, float znear, float zfar);
  void setLookAt(const Vec3& eye, const Vec3& center, const Vec3& up);

  void setAspect(float aspect);

  const Mat4& proj() const;
  const Mat4& view() const;

private:
  Mat4 _proj;
  Mat4 _view;
  float _fov = 0.0f, _znear = 0.0f, _zfar = 0.0f, _aspect = 1.0f;
};

} // namespace scop

#endif // SCOP_CAMERA_HPP
