#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "RtWeekend.h"

class BoundingBox {
    public:
        BoundingBox() {}
        BoundingBox(const Vec3& a, const Vec3& b) { minimum = a; maximum = b;}

        Vec3 min() const {return minimum; }
        Vec3 max() const {return maximum; }

        bool hit(const Ray& r, double tMin, double tMax) const {
            for (int a = 0; a < 3; a++) {
                auto t0 = fmin((minimum[a] - r.origin()[a]) / r.direction()[a],
                               (maximum[a] - r.origin()[a]) / r.direction()[a]);
                auto t1 = fmax((minimum[a] - r.origin()[a]) / r.direction()[a],
                               (maximum[a] - r.origin()[a]) / r.direction()[a]);
                tMin = fmax(t0, tMin);
                tMax = fmin(t1, tMax);
                if (tMax <= tMin)
                    return false;
            }
            return true;
        }

        Vec3 minimum;
        Vec3 maximum;
};

BoundingBox generateSurroundingBox(BoundingBox box0, BoundingBox box1) {
            
            double x,y,z;

            x = fmin(box0.min().x(), box1.min().x());
            y = fmin(box0.min().y(), box1.min().y());
            z = fmin(box0.min().z(), box1.min().z());

            Vec3 min {x, y, z};



            x = fmax(box0.max().x(), box1.max().x());
            y = fmax(box0.max().y(), box1.max().y());
            z = fmax(box0.max().z(), box1.max().z());

            Vec3 max {x, y, z};

            return BoundingBox(min, max);
        }

#endif