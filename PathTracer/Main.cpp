#include <iostream>
#include <float.h>

#include "sphere.h"
#include "hittableList.h"
#include "camera.h"
#include "random.h"
#include "material.h"

/****************************************************************************************
The code for this path tracer is based on "Ray Tracing in One Weekend" by Peter Shirley. 
				https://github.com/RayTracing/raytracing.github.io

Additional/better graphics to illustrate ray tracing from the "1000 Forms of Bunnies" blog.
				http://viclw17.github.io/tag/#/Ray%20Tracing%20in%20One%20Weekend
*****************************************************************************************/



/*
* Assign colors to pixels
*
* Depth is the number of reflections
*
* Background -
* Linearly blends white and blue depending on the value of y coordinate (Linear Blend/Linear Interpolation/lerp).
* Lerps are always of the form: blended_value = (1-t)*start_value + t*end_value.
* t = 0.0 = White
* t = 1.0 = Blue
*/
vec3 color(const ray& r, hittable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, DBL_MAX, rec)) {
        ray scattered;
        vec3 attenuation; 
        if (depth < 50 && rec.material_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth+1);
        }
        else {
            return vec3(0,0,0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

int main() {

	int nx = 150; // Number of horizontal pixels
	int ny = 75; // Number of vertical pixels
	int ns = 100; // Number of samples for each pixel for anti-aliasing (see AntiAliasing.png for visualization)
	std::cout << "P3\n" << nx << " " << ny << "\n255\n"; // P3 signifies ASCII, 255 signifies max color value

	// Create spheres
	hittable *list[5];
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
    list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(vec3(0.1, 0.2, 0.5), 1.5));
	list[4] = new sphere(vec3(-1,.5,-2), -0.45, new dielectric(vec3(0.1, 0.2, 0.2), 1.5));
    hittable *world = new hittable_list(list,5);
	camera cam;

	for (int j = ny - 1; j >= 0; j--) { // Navigate canvas
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) { // Anti-aliasing - get ns samples for each pixel
				double u = (i + random_double(0.0, 0.999)) / double(nx);
				double v = (j + random_double(0.0, 0.999)) / double(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
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