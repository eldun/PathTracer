#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "BoundingBox.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class Material; // forward declaration

struct HitRecord {
	double t; // parameter of the ray that locates the intersection point
	
	// surface coordinates of the hit point(uv texture coordinates)
	double u;
	double v;

	Vec3 p; // intersection point
	Vec3 normal;
	bool frontFace;
	shared_ptr<Material> materialPtr;

	inline void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

/* 
* Objects rays can hit.
*/
class Hittable {
public: 
  	virtual bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;
	virtual BoundingBox getBoundingBox() const = 0;

};

#endif // !HITTABLE_H

