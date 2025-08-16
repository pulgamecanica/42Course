#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
  private:
  double _e[3];
  public:
  vec3() : _e{0,0,0} {}
  vec3(double e0, double e1, double e2) : _e{e0, e1, e2} {}

  double x() const { return _e[0]; }
  double y() const { return _e[1]; }
  double z() const { return _e[2]; }

  vec3 operator-() const { return vec3(-_e[0], -_e[1], -_e[2]); }
  double operator[](int i) const { return _e[i]; }
  double& operator[](int i) { return _e[i]; }

  vec3& operator+=(const vec3& v) {
    _e[0] += v._e[0];
    _e[1] += v._e[1];
    _e[2] += v._e[2];
    return *this;
  }

  vec3& operator*=(double t) {
    _e[0] *= t;
    _e[1] *= t;
    _e[2] *= t;
    return *this;
  }

  vec3& operator/=(double t) {
    return *this *= 1/t;
  }

  double length() const {
    return std::sqrt(length_squared());
  }

  double length_squared() const {
    return _e[0]*_e[0] + _e[1]*_e[1] + _e[2]*_e[2];
  }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
  return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline vec3 operator+(const vec3& u, const vec3& v) {
  return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline vec3 operator-(const vec3& u, const vec3& v) {
  return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline vec3 operator*(const vec3& u, const vec3& v) {
  return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline vec3 operator*(double t, const vec3& v) {
  return vec3(t*v.x(), t*v.y(), t*v.z());
}

inline vec3 operator*(const vec3& v, double t) {
  return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
  return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
  return u.x() * v.x()
        + u.y() * v.y()
        + u.z() * v.z();
}

inline vec3 cross(const vec3& u, const vec3& v) {
  return vec3(u.y() * v.z() - u.z() * v.y(),
              u.z() * v.x() - u.x() * v.z(),
              u.x() * v.y() - u.y() * v.x());
}

inline vec3 unit_vector(const vec3& v) {
  return v / v.length();
}

#endif
