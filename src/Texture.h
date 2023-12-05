#ifndef TEXTURE_H
#define TEXTURE_H

#include "RtWeekend.h"

class Texture {
    public:
        virtual Vec3 value(double u, double v, const Vec3& p) const = 0;
};

class SolidColor : public Texture {
    public:
        SolidColor() {}
        SolidColor(Vec3 c) : colorValue(c) {}

        SolidColor(double red, double green, double blue)
          : SolidColor(Vec3(red,green,blue)) {}

        virtual Vec3 value(double u, double v, const Vec3& p) const override {
            return colorValue;
        }

    private:
        Vec3 colorValue;
};

#endif
