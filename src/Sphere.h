#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "BoundingBox.h"

class Sphere : public Hittable {
public:
	Sphere() {}

	// Stationary Sphere
	Sphere(Vec3 center, float radius, shared_ptr<Material> material) : 
		centerStart(center), 
		centerEnd(center), 
		moveStartTime(0),
		moveEndTime(0),
		radius(radius), 
		materialPtr(material)
		{
			auto rVec = Vec3(radius, radius, radius);
			this->boundingBox = BoundingBox(center - rVec, center + rVec);
		};

	// Moving Sphere
	Sphere(Vec3 centerStart, Vec3 centerEnd, double moveStartTime, double moveEndTime, float radius, shared_ptr<Material> material) : 
		centerStart(centerStart),
		centerEnd(centerEnd),
		moveStartTime(moveStartTime), 
		moveEndTime(moveEndTime),
		radius(radius), 
		materialPtr(material)
		{
			auto rVec = Vec3(radius, radius, radius);
			BoundingBox box1(centerStart - rVec, centerStart + rVec);
			BoundingBox box2(centerEnd - rVec, centerEnd + rVec);
			boundingBox = BoundingBox(box1, box2); 

			centerVec = centerEnd - centerStart;
		};

	bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const override;
	Vec3 centerAt(double time) const;
	BoundingBox getBoundingBox() const override { return boundingBox; }

private:
	Vec3 centerStart, centerVec, centerEnd;
	double moveStartTime, moveEndTime;
	double radius;
	shared_ptr<Material> materialPtr;
	BoundingBox boundingBox;
	static void getUvCoordinates(const Vec3& p, double& u, double& v);
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

bool Sphere::hit(const Ray& r, Interval ray_t, HitRecord& rec) const {
	Vec3 center = centerAt(r.moment());
	Vec3 oc = r.origin() - center; // Vector from center to ray origin
	double a = r.direction().lengthSquared();
	double halfB = dot(oc, r.direction());
	double c = oc.lengthSquared() - radius*radius;
	double discriminant = (halfB * halfB) - (a * c);
	if (discriminant < 0) {
            return false;
	}

		// Find the nearest root that lies in the acceptable range.
        auto sqrtd = sqrt(discriminant);
        auto root = (-halfB - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-halfB + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
		}

		rec.t = root;
        rec.p = r.pointAtParameter(rec.t);
        Vec3 outward_normal = (rec.p - center) / radius;
        rec.setFaceNormal(r, outward_normal);
        getUvCoordinates(outward_normal, rec.u, rec.v);
		rec.materialPtr = materialPtr;

        return true;	
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
		return centerStart + (time * centerVec);
}


void Sphere::getUvCoordinates(const Vec3& p, double& u, double& v){
	// p: a given point on the sphere of radius one, centered at the origin.
	// u: returned value [0,1] of angle around the Y axis from X=-1.	
	// v: returned value [0,1] of angle from Y=-1 to Y=+1.	
	//     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>	
	//     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>	
	//     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>	

	auto theta = acos(-p.y());
	auto phi = atan2(-p.z(), p.x()) + pi;

	u = phi / (2*pi);
	v = theta / pi;
}

#endif // !SPHERE_H