#include "Math.hpp"
#include <cmath>
#include <algorithm>

namespace scop {

// ---- Vec helpers ----
static inline Vec3 vec3_add(const Vec3& a, const Vec3& b) { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
static inline Vec3 vec3_sub(const Vec3& a, const Vec3& b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
static inline Vec3 vec3_scale(const Vec3& a, float s)     { return {a.x*s, a.y*s, a.z*s}; }
static inline float vec3_dot(const Vec3& a, const Vec3& b){ return a.x*b.x + a.y*b.y + a.z*b.z; }
static inline Vec3 vec3_cross(const Vec3& a, const Vec3& b){
  return { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
}
static inline float vec3_len(const Vec3& v){ return std::sqrt(vec3_dot(v,v)); }
static inline Vec3 vec3_norm(const Vec3& v){
  const float l = vec3_len(v);
  return (l > 0.f) ? Vec3{v.x/l, v.y/l, v.z/l} : Vec3{0,0,0};
}

// Column-major helpers
static inline float& M(Mat4& m, int row, int col) { return m.m[col*4 + row]; }
static inline float  M(const Mat4& m, int row, int col) { return m.m[col*4 + row]; }

// ---- Mat4 impl ----
Mat4 Mat4::identity() {
  Mat4 r{};
  M(r,0,0)=1; M(r,1,1)=1; M(r,2,2)=1; M(r,3,3)=1;
  return r;
}

Mat4 Mat4::operator*(const Mat4& b) const {
  Mat4 out{};
  for (int c=0;c<4;++c){
    for (int r=0;r<4;++r){
      float sum=0.f;
      for (int k=0;k<4;++k) sum += M(*this,r,k) * M(b,k,c);
      M(out,r,c)=sum;
    }
  }
  return out;
}

Mat4 Mat4::translate(const Vec3& t){
  Mat4 r = Mat4::identity();
  M(r,0,3)=t.x; M(r,1,3)=t.y; M(r,2,3)=t.z;
  return r;
}

Mat4 Mat4::scale(const Vec3& s){
  Mat4 r{};
  M(r,0,0)=s.x; M(r,1,1)=s.y; M(r,2,2)=s.z; M(r,3,3)=1.f;
  return r;
}

Mat4 Mat4::rotateAxisAngle(const Vec3& axis, float radians){
  const Vec3 a = vec3_norm(axis);
  const float c = std::cos(radians);
  const float s = std::sin(radians);
  const float t = 1.f - c;

  const float x=a.x, y=a.y, z=a.z;

  Mat4 r = Mat4::identity();
  M(r,0,0)=t*x*x + c;     M(r,0,1)=t*x*y - s*z;  M(r,0,2)=t*x*z + s*y;
  M(r,1,0)=t*x*y + s*z;   M(r,1,1)=t*y*y + c;    M(r,1,2)=t*y*z - s*x;
  M(r,2,0)=t*x*z - s*y;   M(r,2,1)=t*y*z + s*x;  M(r,2,2)=t*z*z + c;
  return r;
}

// OpenGL-like perspective (-1..1 clip z). For Vulkan you may adapt later (0..1, Y flip).
Mat4 Mat4::perspective(float fovyRadians, float aspect, float znear, float zfar){
  const float f = 1.f / std::tan(fovyRadians * 0.5f);
  Mat4 r{};
  M(r,0,0)=f/aspect;
  M(r,1,1)=f;
  M(r,2,2)=(zfar+znear)/(znear - zfar);
  M(r,2,3)=(2.f*zfar*znear)/(znear - zfar);
  M(r,3,2)=-1.f;
  return r;
}

Mat4 Mat4::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up){
  const Vec3 f = vec3_norm(vec3_sub(center, eye));   // forward
  const Vec3 s = vec3_norm(vec3_cross(f, up));       // right
  const Vec3 u = vec3_cross(s, f);                   // corrected up

  Mat4 r = Mat4::identity();
  // rotation (basis in columns)
  M(r,0,0)= s.x; M(r,1,0)= s.y; M(r,2,0)= s.z;
  M(r,0,1)= u.x; M(r,1,1)= u.y; M(r,2,1)= u.z;
  M(r,0,2)=-f.x; M(r,1,2)=-f.y; M(r,2,2)=-f.z;
  // translation
  M(r,0,3) = -vec3_dot(s, eye);
  M(r,1,3) = -vec3_dot(u, eye);
  M(r,2,3) =  vec3_dot(f, eye);
  return r;
}

} // namespace scop
