#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdlib.h>
#include <iostream>

// 3 dimensional vectors will be used for colors, locations, directions, offsets, etc.
class Vec3
{
public:
	Vec3() {}
	Vec3(double e0, double e1, double e2)
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}

	inline double x() const { return e[0]; }
	inline double y() const { return e[1]; }
	inline double z() const { return e[2]; }
	inline double r() const { return e[0]; }
	inline double g() const { return e[1]; }
	inline double b() const { return e[2]; }

	// return reference to current Vec3 object
	inline const Vec3 &operator+() const { return *this; }

	// return opposite of vector when using '-'
	inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }

	// return value or reference to value of Vec3 at index i ( I believe)
	inline double operator[](int i) const { return e[i]; }
	inline double &operator[](int i) { return e[i]; };

	inline Vec3 &operator+=(const Vec3 &v2);
	inline Vec3 &operator-=(const Vec3 &v2);
	inline Vec3 &operator*=(const Vec3 &v2);
	inline Vec3 &operator/=(const Vec3 &v2);
	inline Vec3 &operator*=(const double t);
	inline Vec3 &operator/=(const double t);

	inline double length() const
	{
		return sqrt(
			e[0] * e[0] +
			e[1] * e[1] +
			e[2] * e[2]);
	}

	inline double lengthSquared() const
	{
		return e[0] * e[0] +
			   e[1] * e[1] +
			   e[2] * e[2];
	}
	inline void makeUnitVector();

	inline static Vec3 random()
	{
		return Vec3(randomDouble(), randomDouble(), randomDouble());
	}

	inline static Vec3 random(double min, double max)
	{
		return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
	}

	double e[3];
};

// input output overloading
inline std::istream &operator>>(std::istream &is, Vec3 &t)
{
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline std::ostream &operator<<(std::ostream &os, const Vec3 &t)
{
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}

inline void Vec3::makeUnitVector()
{
	double k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
}

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2)
{
	return Vec3(v1.e[0] + v2.e[0],
	 			v1.e[1] + v2.e[1],
	  			v1.e[2] + v2.e[2]);
}

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.e[0] - v2.e[0], 
				v1.e[1] - v2.e[1], 
				v1.e[2] - v2.e[2]);
}

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2)
{
	return Vec3(v1.e[0] * v2.e[0], 
				v1.e[1] * v2.e[1], 
				v1.e[2] * v2.e[2]);
}

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2)
{
	return Vec3(v1.e[0] / v2.e[0], 
				v1.e[1] / v2.e[1], 
				v1.e[2] / v2.e[2]);
}

inline Vec3 operator*(double t, const Vec3 &v)
{
	return Vec3(t * v.e[0], 
				t * v.e[1], 
				t * v.e[2]);
}

inline Vec3 operator/(const Vec3 v, double t)
{
	return Vec3(v.e[0] / t, 
				v.e[1] / t, 
				v.e[2] / t);
}

inline Vec3 operator*(const Vec3 &v, double t)
{
	return Vec3(t * v.e[0], 
				t * v.e[1], 
				t * v.e[2]);
}

// Dot product
inline double dot(const Vec3 &v1, const Vec3 &v2)
{
	return v1.e[0] * v2.e[0] +
		   v1.e[1] * v2.e[1] +
		   v1.e[2] * v2.e[2];
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2)
{
	return Vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
				v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
				v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline Vec3 &Vec3::operator+=(const Vec3 &v)
{
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}
inline Vec3 &Vec3::operator-=(const Vec3 &v)
{
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline Vec3 &Vec3::operator*=(const Vec3 &v)
{
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline Vec3 &Vec3::operator/=(const Vec3 &v)
{
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline Vec3 &Vec3::operator*=(const double t)
{
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline Vec3 &Vec3::operator/=(const double t)
{
	double k = 1.0 / t;

	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

inline Vec3 unitVector(Vec3 v)
{
	return v / v.length();
}

/*
 * Rejection method to determine a random coordinate in the unit Sphere
 * See RejectionSampling.png for a vizualization. This finds the random point S shown in Diffuse.png.
 */
Vec3 randomUnitSphereCoordinate()
{
	while (true) {
        auto p = Vec3::random(-1,1);
        if (p.lengthSquared() < 1)
            return p;
    }
}

/*
 * Used for calculating true lambertian reflections.
 * See here for more:
 * https://eldun.github.io/2020/06/19/ray-tracing-in-one-weekend-part-two.html#true-lambertian-reflection
 */
Vec3 randomUnitVector()
{
	return unitVector(randomUnitSphereCoordinate());
}



// Real cameras are a bit more complicated than will be represented here.
// For blur/DoF, Ray origins will be on a disk rather than on a point.
// See VirtualFilmPlane.png for a visualization.
Vec3 randomUnitDiskCoordinate()
{
	Vec3 p;
	do
	{
		p = 2.0 * Vec3(randomDouble(0, 1), randomDouble(0, 1), 0) - Vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

#endif // !VEC3_H