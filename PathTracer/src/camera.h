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
		p = 2.0 * vec3(random_double(0, 1), random_double(0, 1), random_double(0, 1)) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

// Real cameras are a bit more complicated than will be represented here.
// For blur/DoF, ray origins will be on a disk rather than on a point.
// See VirtualFilmPlane.png for a visualization.
vec3 random_unit_disk_coordinate() {
    vec3 p;
    do {
        p = 2.0*vec3(random_double(0,1),random_double(0,1),0) - vec3(1,1,0);
    } while (dot(p,p) >= 1.0);
    return p;
}

class camera {
    public:
        camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, double vFov, double aspectRatio,
               double aperture, double focusDistance) {
            lensRadius = aperture / 2;
            double theta = vFov*M_PI/180;
            double halfHeight = tan(theta/2);
            double halfWidth = aspectRatio * halfHeight;
            origin = lookFrom;
            w = unit_vector(lookFrom - lookAt);
            u = unit_vector(cross(vUp, w));
            v = cross(w, u);
            lowerLeftCorner = origin
                              - halfWidth * focusDistance * u
                              - halfHeight * focusDistance * v
                              - focusDistance * w;
            horizontal = 2*halfWidth*focusDistance*u;
            vertical = 2*halfHeight*focusDistance*v;
        }

        ray get_ray(double s, double t) {
            vec3 rd = lensRadius*random_unit_disk_coordinate();
            vec3 offset = u * rd.x() + v * rd.y();
            return ray(origin + offset,
                       lowerLeftCorner + s*horizontal + t*vertical
                           - origin - offset);
        }

        vec3 origin;
        vec3 lowerLeftCorner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lensRadius;
};

#endif // !CAMERAH