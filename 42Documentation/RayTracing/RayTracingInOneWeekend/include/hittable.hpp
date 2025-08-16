#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.hpp"
#include "interval.hpp"

class hit_record {
  public:
    point3 _p; // point where it's hit
    vec3 _normal; // normal perperndicular to the surface hit
    double _t; // root of the ray?
    bool _front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
      // Sets the hit record normal vector.
      // NOTE: the parameter `outward_normal` is assumed to have unit length.

      _front_face = dot(r.direction(), outward_normal) < 0;
      _normal = _front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif