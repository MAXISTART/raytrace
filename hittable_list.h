#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H





#include "hittable.h"
#include <vector>
#include <memory>


using std::shared_ptr;


class hittable_list : public hittable {
public:
	std::vector<shared_ptr<hittable>> objects;

	hittable_list() {};
	hittable_list(shared_ptr<hittable> object) { add(object); };

	void add(shared_ptr<hittable> object) {
		objects.push_back(object);
	}
	
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};


bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	bool hit_anything = false;
	auto t_nearest = t_max;
	for (const auto& obj : objects) {
		if (obj->hit(r, t_min, t_nearest, rec)) {
			hit_anything = true;
			t_nearest = rec.t;
		}
	}
	return hit_anything;
};


#endif // !HITTABLE_LIST_H