#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "RtWeekend.h"
#include "Interval.h"

class BoundingBox {
  public:
    Interval x, y, z;

    BoundingBox() {} // The default AABB is empty, since intervals are empty by default.

    BoundingBox(const Interval& intervalX, const Interval& intervalY, const Interval& intervalZ)
      : x(intervalX), y(intervalY), z(intervalZ) { }

    BoundingBox(const Vec3& a, const Vec3& b) {
        // Treat the two points a and b as extrema for the bounding box, so we don't require a
        // particular minimum/maximum coordinate order.
        x = Interval(fmin(a[0],b[0]), fmax(a[0],b[0]));
        y = Interval(fmin(a[1],b[1]), fmax(a[1],b[1]));
        z = Interval(fmin(a[2],b[2]), fmax(a[2],b[2]));
    }

    BoundingBox(const BoundingBox& box0, const BoundingBox& box1) {
        x = Interval(box0.x, box1.x);
        y = Interval(box0.y, box1.y);
        z = Interval(box0.z, box1.z);
    }

    const Interval& axis(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    bool hit(const Ray& r, Interval ray_t) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1 / r.direction()[a];
            auto orig = r.origin()[a];

            auto t0 = (axis(a).min - orig) * invD;
            auto t1 = (axis(a).max - orig) * invD;

            if (invD < 0)
                std::swap(t0, t1);

            if (t0 > ray_t.min) ray_t.min = t0;
            if (t1 < ray_t.max) ray_t.max = t1;

            if (ray_t.max <= ray_t.min)
                return false;
        }
        return true;
    }
};

#endif