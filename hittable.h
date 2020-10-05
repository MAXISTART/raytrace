#ifndef HITTABLE_H
#define HITTABLE_H

#include "material.h"
#include "ray.h"
#include <memory>

using std::shared_ptr;

struct hit_record {
	point3 p;
	vec3 normal;
	double t;
	bool front_face;
	shared_ptr<material> mat_ptr;

	inline void setFrontFace(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.dir, outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	};

};

class hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};



#endif // !HITTABLE_H
