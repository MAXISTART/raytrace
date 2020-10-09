#pragma once
#include "material.h"
#include "hittable.h"

class dielectric : public material {
public:
	double ir;
public:

	dielectric(double index_of_refraction):ir(index_of_refraction){};
	virtual bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

private:
	// 计算反射分量
	static double reflectance(double cosine, double ref_idx) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0)*pow((1 - cosine), 5);
	}
};	

bool dielectric::scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const {
	// 折射不吸收能量
	attenuation = color(1.0, 1.0, 1.0);
	vec3 unit_direction = unit_vector(ray_in.direction());
	// 这里需要注意，front_face == false 则说明这是由内部折射到外面
	double refract_rate = rec.front_face ? 1.0 / ir : ir;
	double cos_theta = fmin(dot(rec.normal, -unit_direction), 1.0);
	double sin_theta = sqrt(1 - cos_theta * cos_theta);
	bool cannot_refract = (sin_theta * refract_rate) > 1 ? true : false;
 

	// 需要判断折射率是否过大或者光束本身是一条反射光束，过大则反射
	// 衡量折射分量和反射分量可以用概率来模拟，因为 random_double 是 uniform distribution， 你发出的光线实际有很多条，给予一定概率是否为反射光线
	// 本质就是模拟有多少反射分量了。
	if (cannot_refract || reflectance(cos_theta, refract_rate) > random_double()) {
		scattered = ray(rec.p, reflect(unit_direction, rec.normal));	
	}
	else {
		scattered = ray(rec.p, refract(unit_direction, rec.normal, refract_rate));
	}
	return true;
}
