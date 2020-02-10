#ifndef SPHEREH
#define SPHEREH

#include "hittable.h"

class sphere : public hittable {
public:
	sphere() {}
	sphere(vec3 cen, float r, material* material) : center(cen), radius(r), material_ptr(material) {};
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	vec3 center;
	double radius;
	material* material_ptr;
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
*/

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center; // Vector from center to ray origin
	double a = dot(r.direction(), r.direction());
	double b = dot(oc, r.direction());
	double c = dot(oc, oc) - radius*radius;
	double discriminant = (b * b) - (a * c);
	if (discriminant > 0.0) {
		double temp = (-b - sqrt(discriminant)) / a; // quadratic
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.material_ptr = material_ptr;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.material_ptr = material_ptr;
			return true;
		}
	}
	return false;
}

#endif // !SPHEREH

