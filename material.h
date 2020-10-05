#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"

struct hit_record;

class material{
public:
	virtual bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};



#endif // !MATERIAL_H
