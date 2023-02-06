#ifndef BVHH
#define BVHH

#include <algorithm>

#include "RtWeekend.h"

#include "Hittable.h"
#include "HittableList.h"


class BvhNode : public Hittable {
    public:
    
        BvhNode();

        BvhNode(const HittableList& list, double timeStart, double timeEnd)
            : BvhNode(list.objects, 0, list.objects.size(), timeStart, timeEnd)
        {}

        BvhNode(
            const std::vector<shared_ptr<Hittable>>& srcObjects,
            size_t start, size_t end, double timeStart, double timeEnd);

        virtual bool hit(
            const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

        virtual bool generateBoundingBox(double timeStart, double timeEnd, BoundingBox& outputBox) const override;

    public:
        shared_ptr<Hittable> left;
        shared_ptr<Hittable> right;
        BoundingBox box;
};

BvhNode::BvhNode(
    const std::vector<shared_ptr<Hittable>>& srcObjects,
    size_t start, size_t end, double timeStart, double timeEnd
) {
    auto objects = srcObjects; // Create a modifiable array of the source scene objects

    int axis = randomInt(0,2);
    auto comparator = (axis == 0) ? xBoxCompare
                    : (axis == 1) ? yBoxCompare
                                  : zBoxCompare;

    size_t objectSpan = end - start;

    if (objectSpan == 1) {
        left = right = objects[start];
    } else if (objectSpan == 2) {
        if (comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + objectSpan/2;
        left = make_shared<BvhNode>(objects, start, mid, timeStart, timeEnd);
        right = make_shared<BvhNode>(objects, mid, end, timeStart, timeEnd);
    }

    BoundingBox boxLeft, boxRight;

    if (  !left->generateBoundingBox (timeStart, timeEnd, boxLeft)
       || !right->generateBoundingBox(timeStart, timeEnd, boxRight)
    )
        std::cerr << "No bounding box in BvhNode constructor.\n";

    box = generateSurroundingBox(boxLeft, boxRight);
}

bool BvhNode::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
    if (!box.hit(r, tMin, tMax))
        return false;

    bool hitLeft = left->hit(r, tMin, tMax, rec);
    bool hitRight = right->hit(r, tMin, hitLeft ? rec.t : tMax, rec);

    return hitLeft || hitRight;
}

bool BvhNode::generateBoundingBox(double timeStart, double timeEnd, BoundingBox& outputBox) const {
    outputBox = box;
    return true;
}

#endif