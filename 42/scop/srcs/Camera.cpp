#include "Camera.hpp"
#include <algorithm>
#include <cmath>

namespace scop {

static inline float vlen(const Vec3& v) {
  return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Camera::Camera() {
  _proj = Mat4::identity();
  _view = Mat4::identity();
}

void Camera::setPerspective(float fovRadians, float aspect, float znear, float zfar) {
  _fov = fovRadians; _aspect = aspect; _znear = znear; _zfar = zfar;
  _proj = Mat4::perspective(_fov, _aspect, _znear, _zfar);
}

void Camera::setLookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
  _eye = eye; _center = center; _up = up;
  _view = Mat4::lookAt(_eye, _center, _up);
}

void Camera::zoom(float factor) {
  // Move the eye along the center->eye direction, scaling its distance. The
  // center is untouched, so the model stays framed exactly where it was.
  Vec3 dir{_eye.x - _center.x, _eye.y - _center.y, _eye.z - _center.z};
  float dist = vlen(dir);
  if (dist <= 1e-6f) return;

  float newDist = std::clamp(dist * factor, 0.2f, 100.0f);
  float s = newDist / dist;
  _eye = {_center.x + dir.x * s, _center.y + dir.y * s, _center.z + dir.z * s};
  _view = Mat4::lookAt(_eye, _center, _up);
}

void Camera::setAspect(float aspect) {
  _aspect = aspect;
  _proj = Mat4::perspective(_fov, _aspect, _znear, _zfar);
}

const Mat4& Camera::proj() const { return _proj; }
const Mat4& Camera::view() const { return _view; }

} // namespace scop
