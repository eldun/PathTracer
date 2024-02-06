#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "RtWeekend.h"

#include "BoundingBox.h"
#include "Hittable.h"

#include <memory>
#include <vector>


class HittableList : public Hittable {
  public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<Hittable> object) {
        objects.push_back(object);
        boundingBox = BoundingBox(boundingBox, object->getBoundingBox());
    }

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
        HitRecord tempRec;
        auto hitAnything = false;
        auto closestSoFar = ray_t.max;

        for (const auto& object : objects) {
            if (object->hit(r, Interval(ray_t.min, closestSoFar), tempRec)) {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }

    BoundingBox getBoundingBox() const override { return boundingBox; }

  private:
    BoundingBox boundingBox;
};


#endif