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
	// ���㷴�����
	static double reflectance(double cosine, double ref_idx) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0)*pow((1 - cosine), 5);
	}
};	

bool dielectric::scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const {
	// ���䲻��������
	attenuation = color(1.0, 1.0, 1.0);
	vec3 unit_direction = unit_vector(ray_in.direction());
	// ������Ҫע�⣬front_face == false ��˵���������ڲ����䵽����
	double refract_rate = rec.front_face ? 1.0 / ir : ir;
	double cos_theta = fmin(dot(rec.normal, -unit_direction), 1.0);
	double sin_theta = sqrt(1 - cos_theta * cos_theta);
	bool cannot_refract = (sin_theta * refract_rate) > 1 ? true : false;
 

	// ��Ҫ�ж��������Ƿ������߹���������һ�������������������
	// ������������ͷ�����������ø�����ģ�⣬��Ϊ random_double �� uniform distribution�� �㷢���Ĺ���ʵ���кܶ���������һ�������Ƿ�Ϊ�������
	// ���ʾ���ģ���ж��ٷ�������ˡ�
	if (cannot_refract || reflectance(cos_theta, refract_rate) > random_double()) {
		scattered = ray(rec.p, reflect(unit_direction, rec.normal));	
	}
	else {
		scattered = ray(rec.p, refract(unit_direction, rec.normal, refract_rate));
	}
	return true;
}
