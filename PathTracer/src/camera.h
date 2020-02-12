#ifndef CAMERAH
#define CAMERAH

#include "random.h"
#include "ray.h"

/*
* Rejection method to determine a random coordinate in the unit sphere
* See RejectionSampling.png for a vizualization. This finds the random point S shown in Diffuse.png.
*/
vec3 random_unit_sphere_coordinate() {
	vec3 p;
	do {
		p = 2.0 * vec3(random_double(0.0, 0.999), random_double(0.0, 0.999), random_double(0.0, 0.999)) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

class camera {
    public:
        camera(double vFov, double aspectRatio) { // vFov is in degrees
            double theta = vFov*M_PI/180; // convert vertical Fov to radians

			// See CameraModelSummary.png for visualization
            double half_height = tan(theta/2);
            double half_width = aspectRatio * half_height;

            lower_left_corner = vec3(-half_width, -half_height, -1.0);
            horizontal = vec3(2*half_width, 0.0, 0.0);
            vertical = vec3(0.0, 2*half_height, 0.0);
            origin = vec3(0.0, 0.0, 0.0);
        }

        ray get_ray(double u, double v) {
            return ray(origin,
                       lower_left_corner + u*horizontal + v*vertical - origin);
        }

        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};

#endif // !CAMERAH