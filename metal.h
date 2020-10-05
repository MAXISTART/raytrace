#pragma once

#include "material.h"
#include "vec3.h"

class metal : public material {
public:
	metal(const color& al, double fz) : albedo(al), fuzzy(fz < 1 ? fz : 1) {};
	virtual bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

public:
	color albedo;
	double fuzzy;
};

bool metal::scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const {

	auto reflect_ray = reflect(unit_vector(ray_in.direction()), rec.normal) + fuzzy * random_in_unit_sphere();
	scattered = ray(rec.p, reflect_ray);
	attenuation = albedo;
	return (dot(reflect_ray, rec.normal) > 0);
}
