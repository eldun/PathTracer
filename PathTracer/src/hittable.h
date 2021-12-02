#ifndef HITTABLEH
#define HITTABLEH

#include "ray.h"

class material; // forward declaration

struct hit_record {
	double t; // parameter of the ray that locates the intersection point
	vec3 p; // intersection point
	vec3 normal;
	bool front_face;
	material* material_ptr;

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
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

