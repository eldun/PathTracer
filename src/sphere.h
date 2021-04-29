#ifndef SPHEREH
#define SPHEREH

#include "hittable.h"

class sphere : public hittable {
public:
	sphere() {}
	sphere(vec3 center, float radius, shared_ptr<material> material) : 
		centerStart(center), 
		centerEnd(center), 
		moveStartTime(0),
		moveEndTime(0),
		radius(radius), 
		material_ptr(material){};

	// Moving sphere
	sphere(vec3 centerStart, vec3 centerEnd, double moveStartTime, double moveEndTime, float radius, shared_ptr<material> material) : 
		centerStart(centerStart),
		centerEnd(centerEnd),
		moveStartTime(moveStartTime), 
		moveEndTime(moveEndTime),
		radius(radius), 
		material_ptr(material){};

	virtual bool hit(const ray &r, double tmin, double tmax, hit_record &rec) const;

	vec3 centerAt(double time) const;

	vec3 centerStart, centerEnd;
	double moveStartTime, moveEndTime;
	double radius;
	shared_ptr<material> material_ptr;
};

/*
* Check to see if a ray hits a sphere with center at *center* and radius *radius*.
*
* Equation of sphere in vector form : dot((p-c, (p-c)) = R^2
* where p is a point on the ray, c is the center of the sphere, and R is the radius of the sphere.
* This can be read as "any point p that satisfies this equation is on the sphere."
* We want to know if our ray ever hits the sphere, so we are looking for any t where the following is true:
* dot((p(t)-c, (p(t)-c)) = R^2
*
* Expanding the full form of the ray p(t), we get:
* dot((a + t*b -c), (a + t*b -c)) = R^2
* or:
* t*t*dot( B , B ) + 2*t*dot( B,A - C ) + dot( A-C,A - C ) - R*R = 0
*
* solving the quadratic equation for the unknown (t), will result
* in a square root that is positive(two solutions), negative(no solutions), or zero(1 solution). See Quadratic.png for a visual.
* I haven't done geometry in a while.
*
* Adding motion blur. Rays from the camera now exist at a point in time. Spheres can now move.
*/

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	vec3 oc = r.origin() - centerAt(r.moment()); // Vector from center to ray origin
	double a = r.direction().length_squared();
	double halfB = dot(oc, r.direction());
	double c = oc.length_squared() - radius*radius;
	double discriminant = (halfB * halfB) - (a * c);
	if (discriminant > 0.0) {
		auto root = sqrt(discriminant);

		auto temp = (-halfB - root) / a;

		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			vec3 outward_normal = (rec.p - centerAt(r.moment())) / radius;
			rec.set_face_normal(r, outward_normal);
			rec.material_ptr = material_ptr;
			return true;
		}
	}
	return false;
}

vec3 sphere::centerAt(double time) const {

	// Prevent divide by zero(naN) for static spheres
	if (moveStartTime == moveEndTime) {
		return centerStart;
	}

	else if (time < moveStartTime){
		return centerStart;
	}

	else if (time > moveEndTime){
		return centerEnd;
	}

	else 
		return centerStart + ((time - moveStartTime) / (moveEndTime-moveStartTime))*(centerEnd - centerStart);	
}

#endif // !SPHEREH