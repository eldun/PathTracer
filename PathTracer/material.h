#ifndef MATERIALH
#define MATERIALH

#include "hittable.h"
#include "camera.h"
#include "random.h"


// Simulate reflection of a metal (see MetalReflectivity.png)
vec3 reflect(const vec3& v, const vec3& n){
    return v - 2*dot(v,n)*n; // v enters the hittable, which is why subtraction is required.
}

class material {
    public:
    virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

// Matte surface
// Light that reflects off a diffuse surface has its direction randomized.
// Light may also be absorbed. See Diffuse.png for illustration and detailed description
class lambertian : public material {
    public:
        lambertian(const vec3& a) : albedo(a){};
        virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
            vec3 target = rec.p + rec.normal + random_unit_sphere_coordinate();
            scattered = ray(rec.p, target - rec.p);
            attenuation = albedo;
            return true;
        }
    vec3 albedo; // reflectivity

};

// Simulate reflection of a metal (see MetalReflectivity.png)
class metal : public material {
    public:
        metal(const vec3& a) : albedo(a) {}
        virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0.0;
    }

    vec3 albedo;
};



#endif // !MATERIALH