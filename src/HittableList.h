#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"

class HittableList : public Hittable {
public:
	HittableList() {}
	HittableList(shared_ptr<Hittable> object) { add(object); }

	void clear() { objects.clear(); }
    void add(shared_ptr<Hittable> object) { objects.push_back(object); }
	virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;
	virtual bool generateBoundingBox(double timeStart, double timeEnd, BoundingBox& outputBox) const override;

	std::vector<shared_ptr<Hittable>> objects;

};

bool HittableList::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
	HitRecord tempHitRec;
	bool hitStatus = false;
	double closestSoFar = tMax;
	for (const auto& object : objects) {

		if (object->hit(r, tMin, closestSoFar, tempHitRec)) {
			hitStatus = true;
			closestSoFar = tempHitRec.t;
			rec = tempHitRec;
		}
	}
	return hitStatus;
	
}

bool HittableList::generateBoundingBox(double timeStart, double timeEnd, BoundingBox &outputBox) const {
	
	if (objects.empty()) {
		return false;
	}

    BoundingBox tempBox;
    bool isFirstBox = true;

    for (const auto& object : objects) {
        if (!object->generateBoundingBox(timeStart, timeEnd, tempBox)) return false;
        outputBox = isFirstBox ? tempBox :  generateSurroundingBox(outputBox, tempBox);
        isFirstBox = false;
    }

    return true;
	
}

#endif // !HITTABLE_LIST_H

