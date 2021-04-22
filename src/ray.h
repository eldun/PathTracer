#ifndef RAYH
#define RAYH
#include "vec3.h"

/*******************************************************************************
* All ray tracers have a ray class, along with what color is seen along a ray.
* A ray can be thought of as the function p(t) = A + t*B ...
* p is a 3d position along a 3d line
* A is the ray origin
* B is the ray direction
* t is a real number, positive or negative. This allows you to traverse the line and face either direction.
*******************************************************************************/
class ray
{
public:
public:
public:
	ray() {}
	ray(const vec3 &a, const vec3 &b, double moment)
	{
		A = a;
		B = b;
		mMoment = moment;
	}
	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	double moment() const { return mMoment; }
	vec3 point_at_parameter(double t) const { return A + t * B; }

	vec3 A;
	vec3 B;
	double mMoment;
};

#endif // !RAYH