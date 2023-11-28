#ifndef CAMERA_H
#define CAMERA_H

#include "RtWeekend.h"

#include "Ray.h"

class Camera
{
public:
    Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 upDirection, double vFov, double aspectRatio,
           double aperture, double focusDistance, double shutterOpenDuration)
    {
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
        this->shutterOpenDuration = shutterOpenDuration;
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
    double shutterOpenDuration;
};

#endif // !CAMERA_H