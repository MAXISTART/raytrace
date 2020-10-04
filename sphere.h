#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : hittable {

public :
	double radius;
	point3 center;

	sphere() : center(point3(0,0,0)), radius(1.0){};
	sphere(point3 cen, double r): center(cen),radius(r) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {

	vec3 oc = r.origin() - center;
	auto a = r.dir.length_squared();
	auto half_b = dot(r.dir, oc);
	auto c = oc.length_squared() - radius * radius;
	auto delta = half_b * half_b - a * c;
	if (delta < 0) return false;
	else {
		auto t = (-half_b - sqrt(delta)) / a;
		if (t > t_min && t < t_max) {
			rec.p = r.at(t);
			rec.normal = (rec.p - center) / radius;
			rec.t = t;
			return true;
		}

		t = (-half_b + sqrt(delta)) / a;
		if (t > t_min && t < t_max) {
			rec.p = r.at(t);
			rec.normal = (rec.p - center) / radius;
			rec.t = t;
			return true;
		}

		return false;
	}
	 

}



#endif // !SPHERE_H
