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
        camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, double vFov, double aspectRatio) { // vFov is in degrees
            vec3 u, v, w;
            
			double theta = vFov*M_PI/180; // Convert vertical Fov to radians

			// See CameraModelSummary.png for visualization
            double half_height = tan(theta/2);
            double half_width = aspectRatio * half_height;

			// See OrthagonalView.png and OrthangonalBasis.png for a visualization
			// of translating the perspective of the original camera using cross products
			// and specifying which direction is up with vUp.
            origin = lookFrom;
            w = unit_vector(lookFrom - lookAt);
            u = unit_vector(cross(vUp, w));
            v = cross(w, u);
            lower_left_corner = origin - half_width*u - half_height*v - w;
            horizontal = 2*half_width*u;
            vertical = 2*half_height*v;
        }

        ray get_ray(double s, double t) {
            return ray(origin,
                       lower_left_corner + s*horizontal + t*vertical - origin);
        }

        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};

#endif // !CAMERAH