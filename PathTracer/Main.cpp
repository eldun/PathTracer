#include <iostream>
#include "sphere.h"
#include "hittableList.h"
#include "camera.h"
#include "random.h"

/****************************************************************************************
The code for this path tracer is based on "Ray Tracing in One Weekend" by Peter Shirley. 
				https://github.com/RayTracing/raytracing.github.io
*****************************************************************************************/


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
vec3 color(const ray& r, hittable * world) {
	hit_record rec;
	if (world->hit(r, 0.0, TMP_MAX, rec)) {
		return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1); // return a vector with values between 0 and 1 (based on xyz) to be converted to rgb values
	}
	else { // background
		vec3 unit_direction = unit_vector(r.direction());
		double t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main() {

	int nx = 200; // Number of horizontal pixels
	int ny = 100; // Number of vertical pixels
	int ns = 100; // Number of samples for each pixel for anti-aliasing (see AntiAliasing.png for visualization)
	std::cout << "P3\n" << nx << " " << ny << "\n255\n"; // P3 signifies ASCII, 255 signifies max color value

	// Create spheres
	hittable *list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hittable* world = new hittable_list(list, 2);
	camera cam;

	for (int j = ny - 1; j >= 0; j--) { // Navigate canvas
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) { // Anti-aliasing - get ns samples for each pixel
				double u = (i + random_double(0.0, 0.999)) / double(nx);
				double v = (j + random_double(0.0, 0.999)) / double(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world);
			}

			col /= double(ns); // Average the color between objects/background
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}