#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"
#include "vec3.hpp"

class sphere : public hittable {
  public:
    sphere(const point3& center, double radius) : _center(center), _radius(std::fmax(0,radius)) {}
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
      vec3 oc = _center - r.origin();
      auto a = r.direction().length_squared();
      auto h = dot(r.direction(), oc);
      auto c = oc.length_squared() - _radius*_radius;

      auto discriminant = h*h - a*c;
      if (discriminant < 0)
        return false;

      auto sqrtd = std::sqrt(discriminant);

      // Find the nearest root that lies in the acceptable range.
      auto root = (h - sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root))
          return false;
      }

      rec._t = root;
      rec._p = r.at(rec._t);
      rec._normal = (rec._p - _center) / _radius;

      return true;
    }

  private:
    point3 _center;
    double _radius;
};

#endif
