#ifndef RAYH
#define RAYH
#include "Vec3.h"

/*******************************************************************************
 * All Ray tracers have a Ray class, along with what color is seen along a Ray.
 * A Ray can be thought of as the function p(t) = A + t*B ...
 * p is a 3d position along a 3d line
 * A is the Ray origin
 * B is the Ray direction
 * t is a real number, positive or negative. This allows you to traverse the line and face either direction.
 *******************************************************************************/
class Ray {
	
	public:
		Ray() {}
		Ray(const Vec3 &a, const Vec3 &b, double moment)
		{
			this->a = a;
			this->b = b;
			mMoment = moment;
		}
		Vec3 origin() const { return a; }
		Vec3 direction() const { return b; }
		double moment() const { return mMoment; }
		Vec3 pointAtParameter(double t) const { return a + t * b; }
	
	private:
		Vec3 a;
		Vec3 b;
		double mMoment;
};

#endif // !RAYH