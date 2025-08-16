#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

class ray {
  public:
    ray() {}

    ray(const point3& origin, const vec3& direction) : _orig(origin), _dir(direction) {}

    const point3& origin() const  { return _orig; }
    const vec3& direction() const { return _dir; }

    point3 at(double t) const {
      return _orig + t*_dir;
    }

  private:
    point3 _orig;
    vec3 _dir;
};

#endif
