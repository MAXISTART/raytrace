#pragma once

#include "material.h"
#include "vec3.h"

class metal : public material {
public:
	metal(const color& al) : albedo(al) {};
	virtual bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

public:
	color albedo;
};

bool metal::scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const {

	auto reflect_ray = reflect(unit_vector(ray_in.direction()), rec.normal);
	scattered = ray(rec.p, reflect_ray);
	attenuation = albedo;
	return (dot(reflect_ray, rec.normal) > 0);
}
