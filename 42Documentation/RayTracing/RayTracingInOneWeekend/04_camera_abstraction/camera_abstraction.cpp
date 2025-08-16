#include "rtweekend.hpp"

#include "camera.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"


int main() {
   hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    camera cam;

    cam._aspect_ratio = 16.0 / 9.0;
    cam._image_width  = 400;
    cam._samples_per_pixel = 100;
    cam.render(world);
}