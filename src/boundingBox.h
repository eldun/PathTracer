#ifndef BOUNDINGBOXH
#define BOUNDINGBOXH

#include "rtweekend.h"

class boundingBox {
    public:
        boundingBox() {}
        boundingBox(const vec3& a, const vec3& b) { minimum = a; maximum = b;}

        vec3 min() const {return minimum; }
        vec3 max() const {return maximum; }

        bool hit(const ray& r, double t_min, double t_max) const {
            for (int a = 0; a < 3; a++) {
                auto t0 = fmin((minimum[a] - r.origin()[a]) / r.direction()[a],
                               (maximum[a] - r.origin()[a]) / r.direction()[a]);
                auto t1 = fmax((minimum[a] - r.origin()[a]) / r.direction()[a],
                               (maximum[a] - r.origin()[a]) / r.direction()[a]);
                t_min = fmax(t0, t_min);
                t_max = fmin(t1, t_max);
                if (t_max <= t_min)
                    return false;
            }
            return true;
        }

        boundingBox surroundingBox(boundingBox box0, boundingBox box1) const {
            
            double x,y,z;

            x = fmin(box0.min().x(), box1.min().x());
            y = fmin(box0.min().y(), box1.min().y());
            z = fmin(box0.min().z(), box1.min().z());

            vec3 min {x, y, z};



            x = fmax(box0.max().x(), box1.max().x());
            y = fmax(box0.max().y(), box1.max().y());
            z = fmax(box0.max().z(), box1.max().z());

            vec3 max {x, y, z};

            return boundingBox(min, max);
        }

        vec3 minimum;
        vec3 maximum;
};

#endif