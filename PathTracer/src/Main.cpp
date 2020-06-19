#include <chrono> // Record elapsed render time
#include <iostream>
#include <iomanip> // Time formatting
#include <float.h>

#include "rtweekend.h"

#include "sphere.h"
#include "hittableList.h"
#include "camera.h"
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

    if (depth <= 0) {
        return vec3(0,0,0);
    }  
    if (world->hit(r, 0.001, DBL_MAX, rec)) {
        ray scattered;
        vec3 attenuation; 
        if (rec.material_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth-1);
        }
        else {
            return vec3(0,0,0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        double t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

hittable *random_scene() {
    int n = 500;
    hittable **list = new hittable*[n+1];
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5, 0.5, 0.5))); // "Ground"
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double randomMaterial = random_double(0,1);
            vec3 center(a+0.9*random_double(0,1),0.2,b+0.9*random_double(0,1));
            if ((center-vec3(4,0.2,0)).length() > 0.9) {
                if (randomMaterial < 0.68) {  // diffuse
                    list[i++] = new sphere(center, 0.2,
                        new lambertian(vec3(random_double(0,1)*random_double(0,1),
                                            random_double(0,1)*random_double(0,1),
                                            random_double(0,1)*random_double(0,1))
                        )
                    );
                }
                else if (randomMaterial < 0.87) { // metal
                    list[i++] = new sphere(center, 0.2,
                            new metal(vec3(0.5*(1 + random_double(0,1)),
                                           0.5*(1 + random_double(0,1)),
                                           0.5*(1 + random_double(0,1))),
                                      0.5*random_double(0,1)));
                }
                else {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(vec3(random_double(0,1),random_double(0,1),random_double(0,1)), 1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(vec3(1.0,1.0,1.0), 1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(1.0, 1.0, 1.0), 0.0));

    return new hittable_list(list,i);
}

int main() {

	int nx = 2000; // Number of horizontal pixels
	int ny = 1000; // Number of vertical pixels
	int ns = 50; // Number of samples for each pixel for anti-aliasing (see AntiAliasing.png for visualization)
    int maxDepth = 50; // Ray bounce limit
	std::cout << "P3\n" << nx << " " << ny << "\n255\n"; // P3 signifies ASCII, 255 signifies max color value

	vec3 lookFrom(13, 2, 3);
	vec3 lookAt(0,0,0);
	vec3 vUp(0,1,0); // determine "up" for the camera
	double distToFocus = (lookFrom-lookAt).length();
	double aperture = 0.05; // bigger = blurrier

    hittable *world = random_scene();

	camera cam(lookFrom, lookAt, vec3(0,1,0), 20,double(nx)/double(ny), aperture, distToFocus);	

   	auto start = std::chrono::high_resolution_clock::now();


	for (int j = ny - 1; j >= 0; j--) { // Navigate canvas
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) { // Anti-aliasing - get ns samples for each pixel
				double u = (i + random_double(0.0, 0.999)) / double(nx);
				double v = (j + random_double(0.0, 0.999)) / double(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, maxDepth);
			}

			col /= double(ns); // Average the color between objects/background
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));  // set gamma to 2
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
    auto stop = std::chrono::high_resolution_clock::now();

	auto hours = std::chrono::duration_cast<std::chrono::hours>(stop - start);
	auto minutes = std::chrono::duration_cast<std::chrono::minutes>(stop - start) - hours;
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(stop - start) - hours - minutes;

    std::cerr << std::fixed << std::setprecision(2) << 
	"\nDone in:" << std::endl << 
	"\t" << hours.count() << " hours" << std::endl <<
	"\t" << minutes.count() << " minutes" << std::endl <<
	"\t" << seconds.count() << " seconds." << std::endl;
}