#ifndef HITTABLEH
#define HITTABLEH

#include "ray.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class material; // forward declaration

struct hit_record {
	double t; // parameter of the ray that locates the intersection point
	vec3 p; // intersection point
	vec3 normal;
	bool frontFace;
	shared_ptr<material> material_ptr;

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        frontFace = dot(r.direction(), outward_normal) < 0;
        normal = frontFace ? outward_normal : -outward_normal;
    }
};

/* 
* A class for objects rays can hit.
*/
class hittable {
public: 
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif // !HITTABLEH

