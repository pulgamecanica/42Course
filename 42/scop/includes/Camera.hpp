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

  // Dolly the eye toward/away from the look-at center along the view axis.
  // factor < 1 zooms in, factor > 1 zooms out; the center stays fixed so the
  // model remains framed. Distance is clamped to keep the target in view.
  void zoom(float factor);

  const Mat4& proj() const;
  const Mat4& view() const;

private:
  Mat4 _proj;
  Mat4 _view;
  float _fov = 0.0f, _znear = 0.0f, _zfar = 0.0f, _aspect = 1.0f;

  // Cached look-at parameters so zoom can recompute the view matrix.
  Vec3 _eye {0, 0, 0}, _center {0, 0, 0}, _up {0, 1, 0};
};

} // namespace scop

#endif // SCOP_CAMERA_HPP
