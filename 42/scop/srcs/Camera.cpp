#include "Camera.hpp"

namespace scop {

Camera::Camera() {
  _proj = Mat4::identity();
  _view = Mat4::identity();
}

void Camera::setPerspective(float fovRadians, float aspect, float znear, float zfar) {
  _fov = fovRadians; _aspect = aspect; _znear = znear; _zfar = zfar;
  _proj = Mat4::perspective(_fov, _aspect, _znear, _zfar);
}

void Camera::setLookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
  _view = Mat4::lookAt(eye, center, up);
}

void Camera::setAspect(float aspect) {
  _aspect = aspect;
  _proj = Mat4::perspective(_fov, _aspect, _znear, _zfar);
}

const Mat4& Camera::proj() const { return _proj; }
const Mat4& Camera::view() const { return _view; }

} // namespace scop
