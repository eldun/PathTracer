#ifndef TEXTURE_H
#define TEXTURE_H

#include "RtWeekend.h"
#include "Interval.h"
#include "rtw_stb_image.h"
#include "Perlin.h"

class Texture {
    public:
        virtual Vec3 value(double u, double v, const Vec3& p) const = 0;
};

class SolidColor : public Texture {
    public:
        SolidColor(Vec3 c) : colorValue(c) {}

        SolidColor(double red, double green, double blue)
          : SolidColor(Vec3(red,green,blue)) {}

        // Return the texture color of the given coordinates
        virtual Vec3 value(double u, double v, const Vec3& p) const override {
            return colorValue;
        }

    private:
        Vec3 colorValue;
};

class CheckerTexture : public Texture {
    public:
        CheckerTexture(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd) 
            :   invertedScale(1.0 / scale), 
                even(even), 
                odd(odd) {}

        CheckerTexture(double scale, Vec3 colorOne, Vec3 colorTwo) 
            :   invertedScale(1.0 / scale), 
                even(make_shared<SolidColor>(colorOne)), 
                odd(make_shared<SolidColor>(colorTwo)) {}

        Vec3 value(double u, double v, const Vec3& p) const override {
            auto xFloor = static_cast<int>(std::floor(invertedScale * p.x()));
            auto yFloor = static_cast<int>(std::floor(invertedScale * p.y()));
            auto zFloor = static_cast<int>(std::floor(invertedScale * p.z()));

            bool isEven = (xFloor + yFloor + zFloor) % 2 ? true : false;

            return isEven ? even->value(u, v, p) : odd->value(u, v, p);
        }

    private:
        double invertedScale;
        shared_ptr<Texture> even;
        shared_ptr<Texture> odd;

};

class ImageTexture : public Texture {
    public:
        ImageTexture(const char* filename) : image(filename) {}

        Vec3 value(double u, double v, const Vec3& p) const override {
            // If we have no texture data, then return solid cyan as a debugging aid.
            if (image.height() <= 0) return Vec3(0,1,1);

            // Clamp input texture coordinates to [0,1] x [1,0]
            u = Interval(0,1).clamp(u);
            v = 1.0 - Interval(0,1).clamp(v);  // Flip V to image coordinates

            auto i = static_cast<int>(u * image.width());
            auto j = static_cast<int>(v * image.height());
            auto pixel = image.pixel_data(i,j);

            auto colorScale = 1.0 / 255.0;
            return Vec3(colorScale*pixel[0], colorScale*pixel[1], colorScale*pixel[2]);
        }

    private:
        rtw_image image;
};

class NoiseTexture : public Texture {
    public:
        NoiseTexture() {}

        Vec3 value(double u, double v, const Vec3& p) const override {
            return Vec3(1,1,1) * perlin.getNoise(p);
        }

    private:
        Perlin perlin;

};

#endif
