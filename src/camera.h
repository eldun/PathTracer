#ifndef CAMERAH
#define CAMERAH

#include "rtweekend.h"

#include "ray.h"

class camera
{
public:
    camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, double vFov, double aspectRatio,
           double aperture, double focusDistance, double shutterOpenTime, double shutterCloseTime)
    {
        lensRadius = aperture / 2;
        double theta = vFov * pi / 180;
        double halfHeight = tan(theta / 2);
        double halfWidth = aspectRatio * halfHeight;
        origin = lookFrom;
        w = unit_vector(lookFrom - lookAt);
        u = unit_vector(cross(vUp, w));
        v = cross(w, u);
        lowerLeftCorner = origin - halfWidth * focusDistance * u - halfHeight * focusDistance * v - focusDistance * w;
        horizontal = 2 * halfWidth * focusDistance * u;
        vertical = 2 * halfHeight * focusDistance * v;
        this->shutterOpenTime = shutterOpenTime;
        this->shutterCloseTime = shutterCloseTime;
    }

    ray get_ray(double s, double t)
    {
        vec3 rd = lensRadius * random_unit_disk_coordinate();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset,
                   lowerLeftCorner + s * horizontal + t * vertical - origin - offset,
                   random_double(shutterOpenTime, shutterCloseTime));
    }

private:
    vec3 origin;
    vec3 lowerLeftCorner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lensRadius;
    double shutterOpenTime;
    double shutterCloseTime;
};

#endif // !CAMERAH