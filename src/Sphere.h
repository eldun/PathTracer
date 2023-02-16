#ifndef SPHEREH
#define SPHEREH

#include "Hittable.h"

class Sphere : public Hittable {
public:
	Sphere() {}
	Sphere(Vec3 center, float radius, shared_ptr<Material> material) : 
		centerStart(center), 
		centerEnd(center), 
		moveStartTime(0),
		moveEndTime(0),
		radius(radius), 
		materialPtr(material){};

	// Moving Sphere
	Sphere(Vec3 centerStart, Vec3 centerEnd, double moveStartTime, double moveEndTime, float radius, shared_ptr<Material> material) : 
		centerStart(centerStart),
		centerEnd(centerEnd),
		moveStartTime(moveStartTime), 
		moveEndTime(moveEndTime),
		radius(radius), 
		materialPtr(material){};

	virtual bool hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const;
	virtual bool generateBoundingBox(double timeStart, double timeEnd, BoundingBox& outputBox) const override;

	Vec3 centerAt(double time) const;

	Vec3 centerStart, centerEnd;
	double moveStartTime, moveEndTime;
	double radius;
	shared_ptr<Material> materialPtr;
};

/*
* Check to see if a ray hits a Sphere with center at *center* and radius *radius*.
*
* Equation of Sphere in vector form : dot((p-c, (p-c)) = R^2
* where p is a point on the ray, c is the center of the Sphere, and R is the radius of the Sphere.
* This can be read as "any point p that satisfies this equation is on the Sphere."
* We want to know if our ray ever hits the Sphere, so we are looking for any t where the following is true:
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

bool Sphere::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const
{
	Vec3 oc = r.origin() - centerAt(r.moment()); // Vector from center to ray origin
	double a = r.direction().lengthSquared();
	double halfB = dot(oc, r.direction());
	double c = oc.lengthSquared() - radius*radius;
	double discriminant = (halfB * halfB) - (a * c);
	if (discriminant > 0.0) {
		auto root = sqrt(discriminant);

		auto temp = (-halfB - root) / a;

		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.pointAtParameter(rec.t);
			Vec3 outwardNormal = (rec.p - centerAt(r.moment())) / radius;
			rec.setFaceNormal(r, outwardNormal);
			rec.materialPtr = materialPtr;
			return true;
		}
	}
	return false;
}

bool Sphere::generateBoundingBox(double timeStart, double timeEnd, BoundingBox& outputBox) const {
	
	
	BoundingBox box0 = BoundingBox(
			centerAt(timeStart) - Vec3(radius, radius, radius),
			centerAt(timeStart) + Vec3(radius, radius, radius));

	// Sphere is not moving
	if (timeStart-timeEnd < epsilon ) {
		outputBox = box0;
		return true;
	}

	else {
		BoundingBox box1 = BoundingBox(
			centerAt(timeEnd) - Vec3(radius, radius, radius),
			centerAt(timeEnd) + Vec3(radius, radius, radius));
		
		outputBox =  generateSurroundingBox(box0, box1);
		return true;
	}

	
}

Vec3 Sphere::centerAt(double time) const {

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