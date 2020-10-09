#ifndef VEC3
#define VEC3

#include <cmath>
#include <iostream>	
#include <fstream>
#include "tools.h"

class vec3 {
public:
	vec3() : e{ 0,0,0 } {};
	vec3(double e1, double e2, double e3) { e[0] = e1; e[1] = e2; e[2] = e3; };
	vec3(const vec3& v) { e[0] = v.e[0]; e[1] = v.e[1]; e[2] = v.e[2]; };
	

	double x() const { return e[0]; };
	double y() const { return e[1]; };
	double z() const { return e[2]; };

	vec3 operator-() const{ return vec3(-e[0], -e[1], -e[2]); };
	double operator[](int i) const { return e[i]; };
	double& operator[](int i) { return e[i]; };


	// +=,*=,/=
	vec3& operator+=(vec3 v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	};
	vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	vec3& operator/=(const double t) {
		return *this *= 1 / t;
	}

	double length() const{
		return sqrt(length_squared());
	}

	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}


	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double t_min, double t_max) {
		return vec3(random_double(t_min, t_max), random_double(t_min, t_max), random_double(t_min, t_max));
	}

	
public:
	double e[3];

};

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
	return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
	return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
	return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
	return v / v.length();
}

inline vec3 random_in_unit_sphere() {
	vec3 v;
	while (true) {
		v = vec3::random(-1, 1);
		if (v.length_squared() <= 1) break;
	}
	return v;
}

// ·´Éä
inline vec3 reflect(const vec3& in, const vec3& normal) {
	// all is unit_vector!
	return in - 2 * dot(in, normal) * normal;
}

// ÕÛÉä
inline vec3 refract(const vec3& in, const vec3& normal, double refract_rate) {
	auto cos_theta = dot(in, normal);
	auto r_out_perp = refract_rate * (in - cos_theta * normal);
	auto r_out_parallel = -sqrt(abs(1 - r_out_perp.length_squared())) * normal;
	return r_out_perp + r_out_parallel;
}


inline vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}


// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color




#endif // !VEC3
