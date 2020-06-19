#ifndef CAMERAH
#define CAMERAH

#include "rtweekend.h"

#include "ray.h"

class camera {
    public:
        camera(vec3 look_from, vec3 look_at, vec3 vUp, double vFov, double aspect_ratio, double aperture, double focus_distance) {
            
            lens_radius = aperture / 2;
            
            double theta = vFov*pi/180;
            double half_height = tan(theta/2);
            double half_width = aspect_ratio * half_height;
            origin = look_from;
            
            w = unit_vector(look_from - look_at);
            u = unit_vector(cross(vUp, w));
            v = cross(w, u);

            lower_left_corner = origin
                              - half_width * focus_distance * u
                              - half_height * focus_distance * v
                              - focus_distance * w;
            horizontal = 2*half_width*focus_distance*u;
            vertical = 2*half_height*focus_distance*v;
        }

        ray get_ray(double s, double t) {
            vec3 rd = lens_radius*random_unit_disk_coordinate();
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(origin + offset,
                       lower_left_corner + s*horizontal + t*vertical - origin - offset);
        }

        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lens_radius;
};

#endif // !CAMERAH