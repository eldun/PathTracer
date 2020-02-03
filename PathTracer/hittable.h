#ifndef HITTABLEH
#define HITTABLEH

#include "ray.h"

class material; // forward declaration

struct hit_record {
	double t; // parameter of the ray that locates the intersection point
	vec3 p; // intersection point
	vec3 normal;
	material* material_ptr;
};

/* 
* A class for objects rays can hit.
*/
class hittable {
public: 
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif // !HITTABLEH

