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

	// The values below are derived from making the "camera" / ray origin coordinates(0, 0, 0) relative to the canvas.
	// See the included file "TracingIllustration.png" for a visual representation.
	camera() {
		lower_left_corner = vec3(-2.0, -1.0, -1.0);
		horizontal = vec3(4.0, 0.0, 0.0);
		vertical = vec3(0.0, 2.0, 0.0);
		origin = vec3(0.0, 0.0, 0.0);
	}
	ray get_ray(double u, double v) { return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin); }

	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
};

#endif // !CAMERAH