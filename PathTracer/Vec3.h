#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

// 3 dimensional vectors will be used for colors, locations, directions, offsets, etc.
class vec3 {
public:
	vec3() {}
	vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

	// inline to define globals within header
	// const to ensure member variables can't be changed
	inline double x() const { return e[0]; }
	inline double y() const { return e[1]; }
	inline double z() const { return e[2]; }
	inline double r() const { return e[0]; }
	inline double g() const { return e[1]; }
	inline double b() const { return e[2]; }

	// return reference to current vec3 object
	inline const vec3& operator+() const { return *this; }

	// return opposite of vector when using '-'
	inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

	// return value or reference to value of vec3 at index i ( I believe)
	inline double operator[](int i) const { return e[i]; }
	inline double& operator[](int i) { return e[i]; };

	inline vec3& operator+=(const vec3& v2);
	inline vec3& operator-=(const vec3& v2);
	inline vec3& operator*=(const vec3& v2);
	inline vec3& operator/=(const vec3& v2);
	inline vec3& operator*=(const double t);
	inline vec3& operator/=(const double t);

	inline double length() const {
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline double squared_length() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	inline void make_unit_vector();

	double e[3];
};