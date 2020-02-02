#include <iostream>
#include <float.h>
#include "sphere.h"
#include "hittableList.h"
#include "camera.h"
#include "random.h"

/****************************************************************************************
The code for this path tracer is based on "Ray Tracing in One Weekend" by Peter Shirley. 
				https://github.com/RayTracing/raytracing.github.io

Additional/better graphics to illustrate ray tracing from the "1000 Forms of Bunnies" blog.
				http://viclw17.github.io/tag/#/Ray%20Tracing%20in%20One%20Weekend
*****************************************************************************************/

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
	// Light that reflects off a diffuse surface has its direction randomized.
	// Light may also be absorbed. See Diffuse.png for illustration and detailed description.
	if (world->hit(r, 0.01, DBL_MAX, rec)) { // prevent shadow acne due to floating point approximation. Ignore hits very close to zero.
		vec3 target = rec.p + rec.normal + random_unit_sphere_coordinate();
		return 0.5 * color(ray(rec.p, target - rec.p), world); // 0.5 is the light-bounce rate, to simulate absorption.
	}
	else { // background
		vec3 unit_direction = unit_vector(r.direction());
		double t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main() {

	int nx = 600; // Number of horizontal pixels
	int ny = 300; // Number of vertical pixels
	int ns = 50; // Number of samples for each pixel for anti-aliasing (see AntiAliasing.png for visualization)
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
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));  // set gamma to 2
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}