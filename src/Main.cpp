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
#include "Texture.h"

/****************************************************************************************
The code for this path tracer is based on "Ray Tracing in One Weekend" by Peter Shirley.
				https://github.com/RayTracing/raytracing.github.io

Additional/better graphics to illustrate Ray tracing from the "1000 Forms of Bunnies" blog.
				http://viclw17.github.io/tag/#/Ray%20Tracing%20in%20One%20Weekend
*****************************************************************************************/


void generateMovingSphereComparisonScene() {
    HittableList world;

    auto groundMaterial = make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5));
    auto groundSphere = make_shared<Sphere>(Vec3(0,-1000,0), 1000, groundMaterial);

    world.add(groundSphere);

    world.add(make_shared<Sphere>(Vec3(0, 1, 0), 1.0, make_shared<Dielectric>(Vec3(0.9,0.9,0.0), 1.5)));
    world.add(make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1))));
    world.add(make_shared<Sphere>(Vec3(4, 1, 0), 1.0, make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0)));

    // Moving Sphere
    world.add(make_shared<Sphere>(Vec3(-4, 3, 0), Vec3(4,3,0),.25, .75, 1.0, make_shared<Lambertian>(Vec3(0.0, 0.0, 0.0))));

    // BVH
    world = HittableList(make_shared<BvhNode>(world, 0.0, 1.0));

    Camera cam;
    cam.render(world);
}

void generateRandomScene(bool useBvh = true) {
    HittableList world;

    // auto groundMaterial = make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5));
    // world.add(make_shared<Sphere>(Vec3(0,-1000,0), 1000, groundMaterial));

    auto checkerPattern = make_shared<CheckerTexture>(0.32, Vec3(.2, .3, .1), Vec3(.9, .9, .9));
    world.add(make_shared<Sphere>(Vec3(0,-1000,0), 1000, make_shared<Lambertian>(checkerPattern)));


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
        world = HittableList(make_shared<BvhNode>(world, 0.0, 1.0));

    Camera cam;
    cam.render(world);

}

int main() {

    switch (0) {
    case 0:
        generateRandomScene();
        break;

    case 1:
        generateMovingSphereComparisonScene();
        break;

    default:
        generateRandomScene();
        break;
    }

    return 0;
   
}