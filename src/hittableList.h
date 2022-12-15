#ifndef HITTABLELISTH
#define HITTABLELISTH

#include "hittable.h"

class hittableList : public hittable {
public:
	hittableList() {}
	hittableList(shared_ptr<hittable> object) {  }

	void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;

	std::vector<shared_ptr<hittable>> objects;

};

bool hittableList::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (const auto& object : objects) {

		if (object->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}

#endif // !HITTABLELISTH

