#ifndef CAMERA_H
#define CAMERA_H

#include "RtWeekend.h"

#include "Ray.h"
#include "Material.h"

class Camera {
public:

    int imageWidth = 1600; // Number of horizontal pixels
	int samplesPerPixel = 100; // Number of samples for each pixel for anti-aliasing (see AntiAliasing.png for visualization)
    int maxDepth = 40; // Ray bounce limit

	Vec3 lookFrom = Vec3(13,2,3);
	Vec3 lookAt = Vec3(0, 0, 0);
    Vec3 upDirection = Vec3(0,1,0);
    double vFov = 20;
    double aspectRatio = 16.0 / 9.0;
	double focusDistance = (lookFrom-lookAt).length();
    double shutterOpenDuration = 1.0;
	double aperture = 0.1; // bigger = blurrier

    void render(Hittable& world) {
        initialize();


        // .ppm header 
        std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n"; // - P3 signifies ASCII, 255 signifies max color value


        auto start = std::chrono::high_resolution_clock::now();


        for (int j = imageHeight - 1; j >= 0; j--) { // Navigate canvas
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            for (int i = 0; i < imageWidth; i++) {
                Vec3 col(0, 0, 0);
                for (int s = 0; s < samplesPerPixel; s++) { // Anti-aliasing - get ns samples for each pixel
                    double u = (i + randomDouble(0.0, 0.999)) / double(imageWidth);
                    double v = (j + randomDouble(0.0, 0.999)) / double(imageHeight);
                    Ray r = getRay(u, v);
                    col += color(r, world, maxDepth);
                }

                col /= double(samplesPerPixel); // Average the color between objects/background
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

    Ray getRay(double s, double t)
    {
        Vec3 rd = lensRadius * randomUnitDiskCoordinate();
        Vec3 offset = u * rd.x() + v * rd.y();
        return Ray(origin + offset,
                   lowerLeftCorner + s * horizontal + t * vertical - origin - offset,
                   randomDouble(0, shutterOpenDuration));
    }

private:
    Vec3 origin;
    Vec3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    double lensRadius;
    double imageHeight;

    void initialize() {
        imageHeight = static_cast<int>(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        lensRadius = aperture / 2;
        double theta = vFov * pi / 180;
        double halfHeight = tan(theta / 2);
        double halfWidth = aspectRatio * halfHeight;
        origin = lookFrom;
        w = unitVector(lookFrom - lookAt);
        u = unitVector(cross(upDirection, w));
        v = cross(w, u);
        lowerLeftCorner = origin - halfWidth * focusDistance * u - halfHeight * focusDistance * v - focusDistance * w;
        horizontal = 2 * halfWidth * focusDistance * u;
        vertical = 2 * halfHeight * focusDistance * v;
    }

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

        // Linear interpolation (sky)
        else {
            Vec3 unitDirection = unitVector(r.direction());
            double t = 0.5*(unitDirection.y() + 1.0);
            return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
        }
    }
};

#endif // !CAMERA_H