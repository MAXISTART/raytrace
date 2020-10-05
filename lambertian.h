#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.h"
#include "vec3.h"

class lambertian : public material {
public:
	lambertian(const color& al) : albedo(al){};
	virtual bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
public:
	color albedo;
};

bool lambertian::scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const{

	auto reflect_ray = rec.normal + random_in_unit_sphere();
	scattered = ray(rec.p, reflect_ray);
	attenuation = albedo;
	return true;
}


#endif // !LAMBERTIAN_H
