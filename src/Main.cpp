#include <chrono> // Record elapsed render time
#include <iostream>
#include <iomanip> // Time formatting
#include <float.h>

#include "RtWeekend.h"

#include "BvhNode.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"
#include "Material.h"

/****************************************************************************************
The code for this path tracer is based on "Ray Tracing in One Weekend" by Peter Shirley. 
				https://github.com/RayTracing/raytracing.github.io

Additional/better graphics to illustrate Ray tracing from the "1000 Forms of Bunnies" blog.
				http://viclw17.github.io/tag/#/Ray%20Tracing%20in%20One%20Weekend
*****************************************************************************************/



/*
* Assign colors to pixels
*
* Depth is the number of reflections
*
* Background -
* Linearly blends white and blue depending on the value of y coordinate (Linear Blend/Linear Interpolation/lerp).
* Lerps are always of the form: blendedValue = (1-t)*startValue + t*endValue.
* t = 0.0 = White
* t = 1.0 = Blue
*/
Vec3 color(const Ray& r, const Hittable& world, int depth) {
    HitRecord rec;

    if (depth <= 0) {
        return Vec3(0,0,0);
    }  
    if (world.hit(r, 0.001, DBL_MAX, rec)) {
        Ray scattered;
        Vec3 attenuation; 
        if (rec.materialPtr->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth-1);
        }
        else {
            return Vec3(0,0,0);
        }
    }
    else {
        Vec3 unitDirection = unitVector(r.direction());
        double t = 0.5*(unitDirection.y() + 1.0);
        return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
    }
}

HittableList generateMovingSphereComparisonScene() {
    HittableList world;
    
    auto groundMaterial = make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5));
    auto groundSphere = make_shared<Sphere>(Vec3(0,-1000,0), 1000, groundMaterial);

    world.add(groundSphere);

    world.add(make_shared<Sphere>(Vec3(0, 1, 0), 1.0, make_shared<Dielectric>(Vec3(0.9,0.9,0.0), 1.5)));
    world.add(make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1))));
    world.add(make_shared<Sphere>(Vec3(4, 1, 0), 1.0, make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0)));

    // Moving Sphere
    world.add(make_shared<Sphere>(Vec3(-4, 3, 0), Vec3(4,3,0),.25, .75, 1.0, make_shared<Lambertian>(Vec3(0.0, 0.0, 0.0))));


    return world;
}

HittableList generateRandomScene(bool useBvh = true) {
    HittableList world;

    auto groundMaterial = make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Vec3(0,-1000,0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto materialChance = randomDouble();
            Vec3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());

            if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphereMaterial;

                if (materialChance < 0.8) {
                    // diffuse
                    auto albedo = Vec3::random() * Vec3::random();
                    sphereMaterial= make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                } else if (materialChance < 0.95) {
                    // metal
                    auto albedo = Vec3::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                } else {
                    // glass
                    sphereMaterial = make_shared<Dielectric>(Vec3(0.9, 0.9, 0.9), 1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(Vec3(0.9, 0.9, 0.9), 1.5);
    world.add(make_shared<Sphere>(Vec3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Vec3(4, 1, 0), 1.0, material3));

    if (useBvh)
        return HittableList(make_shared<BvhNode>(world, 0.0, 1.0));
        
    return world;

}

int main() {

	int nx = 1600; // Number of horizontal pixels
	int ny = 900; // Number of vertical pixels
	int ns = 60; // Number of samples for each pixel for anti-aliasing (see AntiAliasing.png for visualization)
    int maxDepth = 20; // Ray bounce limit
	std::cout << "P3\n" << nx << " " << ny << "\n255\n"; // P3 signifies ASCII, 255 signifies max color value

	Vec3 lookFrom(0, 2, 24);
	Vec3 lookAt(0, 1, 0);
	double distToFocus = (lookFrom-lookAt).length();
	double aperture = 0.1; // bigger = blurrier

	// auto world = generateMovingSphereComparisonScene();
    auto world = generateRandomScene();

	Camera cam(lookFrom, lookAt, Vec3(0,1,0), 20,double(nx)/double(ny), aperture, distToFocus, 1);	

   	auto start = std::chrono::high_resolution_clock::now();


	for (int j = ny - 1; j >= 0; j--) { // Navigate canvas
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < nx; i++) {
			Vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) { // Anti-aliasing - get ns samples for each pixel
				double u = (i + randomDouble(0.0, 0.999)) / double(nx);
				double v = (j + randomDouble(0.0, 0.999)) / double(ny);
				Ray r = cam.getRay(u, v);
				col += color(r, world, maxDepth);
			}

			col /= double(ns); // Average the color between objects/background
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));  // set gamma to 2
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
	"\nFinished in:" << std::endl << 
	"\t" << hours.count() << " hours" << std::endl <<
	"\t" << minutes.count() << " minutes" << std::endl <<
	"\t" << seconds.count() << " seconds." << std::endl;
}