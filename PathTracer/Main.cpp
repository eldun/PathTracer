#include <iostream>
#include "ray.h"

/****************************************************************************************
The code for this path tracer is based on "Ray Tracing in One Weekend" by Peter Shirley. 
				https://github.com/RayTracing/raytracing.github.io
*****************************************************************************************/

/* 
* Check to see if a ray hits a sphere with center at *center* and radius *radius*. 
*
* Equation of sphere in vector form : dot((p-c, (p-c)) = R^2 
* where p is a point on the ray, c is the center of the sphere, and R is the radius of the sphere.
* This can be read as "any point p that satisfies this equation is on the sphere."
* We want to know if our ray ever hits the sphere, so we are looking for any t where the following is true:
* dot((p(t)-c, (p(t)-c)) = R^2 
*
* Expanding the full form of the ray p(t), we get:
* dot((a + t*b -c), (a + t*b -c)) = R^2
* or:
* t*t*dot( B , B ) + 2*t*dot( B,A - C ) + dot( A-C,A - C ) - R*R = 0
*
* solving the quadratic equation for the unknown (t), will result 
* in a square root that is positive(two solutions), negative(no solutions), or zero(1 solution). See Quadratic.png for a visual.
* I haven't done geometry in a while.
*/
float hit_sphere(const vec3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	double a = dot(r.direction(), r.direction());
	double b = 2.0 * dot(oc, r.direction());
	double c = dot(oc, oc) - radius * radius;
	double discriminant = (b*b) - (4*a*c);
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
}

/*
* Assign colors to pixels
*
* Background -
* Linearly blends white and blue depending on the value of y coordinate (Linear Blend/Linear Interpolation/lerp).
* Lerps are always of the form: blended_value = (1-t)*start_value + t*end_value.
* t = 0.0 = White
* t = 1.0 = Blue
* 
* Draw sphere and surface normals
*/
vec3 color(const ray& r) {
	double t = hit_sphere(vec3(0, 0, -1), 0.5, r); // does the ray hit the values of a sphere placed at (0,0,-1) with a radius of .5?
	if (t > 0.0) { // sphere hit
		vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1)); // N (the normal) is calculated (see SurfaceNormal.png)
		return 0.5 * (vec3(N.x() + 1, N.y() + 1, N.z() + 1)); // RGB values assigned based on xyz values
	}
	vec3 unit_direction = unit_vector(r.direction());
	t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);

}

int main() {
	int nx = 200; // Number of horizontal pixels
	int ny = 100; // Number of vertical pixels
	std::cout << "P3\n" << nx << " " << ny << "\n255\n"; // P3 signifies ASCII, 255 signifies max color value

	// The values below are derived from making the "camera"/ray origin coordinates (0, 0, 0) relative to the canvas. 
	// See the included file "TracingIllustration.png" for a visual representation.
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	for (int j = ny - 1; j >= 0; j--) { // RGB triplets
		for (int i = 0; i < nx; i++) {
			double u = double(i) / double(nx);
			double v = double(j) / double(ny);

			// Approximate pixel centers on the canvas for each ray r
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);

			vec3 col = color(r);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}