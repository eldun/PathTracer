#ifndef BVH_NODE_H
#define BVH_NODE_H

#include <algorithm>

#include "RtWeekend.h"

#include "Hittable.h"
#include "HittableList.h"


class BvhNode : public Hittable {
  public:

    BvhNode(const HittableList& list) : BvhNode(list.objects, 0, list.objects.size()) {}

    BvhNode(const std::vector<shared_ptr<Hittable>>& src_objects, size_t start, size_t end) {
        auto objects = src_objects; // Create a modifiable array of the source scene objects

        int axis = randomInt(0,2);
        auto comparator = (axis == 0) ? boxCompareX
                        : (axis == 1) ? boxCompareY
                                      : boxCompareZ;

        size_t object_span = end - start;

        if (object_span == 1) {
            left = right = objects[start];
        } else if (object_span == 2) {
            if (comparator(objects[start], objects[start+1])) {
                left = objects[start];
                right = objects[start+1];
            } else {
                left = objects[start+1];
                right = objects[start];
            }
        } else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span/2;
            left = make_shared<BvhNode>(objects, start, mid);
            right = make_shared<BvhNode>(objects, mid, end);
        }

        boundingBox = BoundingBox(left->getBoundingBox(), right->getBoundingBox());
    }

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
        if (!boundingBox.hit(r, ray_t))
            return false;

        bool hit_left = left->hit(r, ray_t, rec);
        bool hit_right = right->hit(r, Interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

        return hit_left || hit_right;
    }

    BoundingBox getBoundingBox() const override { return boundingBox; }

  private:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    BoundingBox boundingBox;

    static bool boxCompare(
        const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis_index
    ) {
        return a->getBoundingBox().axis(axis_index).min < b->getBoundingBox().axis(axis_index).min;
    }

    static bool boxCompareX (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        return boxCompare(a, b, 0);
    }

    static bool boxCompareY (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        return boxCompare(a, b, 1);
    }

    static bool boxCompareZ (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        return boxCompare(a, b, 2);
    }
};

#endif