#ifndef MATERIALH
#define MATERIALH

#include "hittable.h"
#include "camera.h"
#include "random.h"


// Simulate reflectivity of a metal surface (see MetalReflectivity.png)
vec3 reflect(const vec3& v, const vec3& n){
    return v - 2*dot(v,n)*n; // v enters the hittable, which is why subtraction is required.
}

// Simulate refraction of light through an object (See RefractiveIndex.png and SnellsLaw.png)
bool refract(const vec3& v, const vec3& n, double ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    double dt = dot(uv, n);
    double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1-dt*dt);

    
    if(discriminant > 0) {
        refracted = ni_over_nt * (uv-n*dt) - n*sqrt(discriminant);
        return true;
    }
    else {
    // either the discriminant is negative, which represents internal reflection,
    // or it's zero, where the refracted ray is perpendicular to the normal.
        return false; 
    }
}

// Schlick's approximation of Fresnel Equations for partial reflectance 
float schlick(float cosine, float ref_idx) {
    float r0 = (1 - ref_idx) / (1 + ref_idx); // ref_idx = n2/n1
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class material {
    public:
    virtual bool scatter(const ray& ray_in, 
                        const hit_record& rec, 
                        vec3& attenuation,
                        ray& scattered) const = 0;
};

// Matte surface
// Light that reflects off a diffuse surface has its direction randomized.
// Light may also be absorbed. See Diffuse.png for illustration and detailed description
class lambertian : public material {
    public:
        lambertian(const vec3& a) : albedo(a){};
        virtual bool scatter(const ray& ray_in, 
                            const hit_record& rec, 
                            vec3& attenuation, 
                            ray& scattered) const {
            vec3 target = rec.p + rec.normal + random_unit_sphere_coordinate();
            scattered = ray(rec.p, target - rec.p);
            attenuation = albedo;
            return true;
        }
    vec3 albedo; // reflectivity

};

// Simulate reflection of a metal (see MetalReflectivity.png)
// See FuzzyReflections.png for a visualization of fuzziness.
class metal : public material {
    public:
        metal(const vec3& a, double f) : albedo(a) {
            if (f<1) fuzz = f; else fuzz = 1; // max fuzz of 1, for now.
        }
        virtual bool scatter(const ray& ray_in, 
                            const hit_record& rec, 
                            vec3& attenuation, 
                            ray& scattered) const {
        vec3 reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_unit_sphere_coordinate()); // large spheres or grazing rays may go below the surface. In that case, they'll just be absorbed.
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0.0;
    }

    vec3 albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(const vec3& a, double ri) : albedo(a), ref_index(ri) {}

    bool scatter(const ray& ray_in,
             const hit_record& rec,
             vec3& attenuation,
             ray& scattered
             ) const {

                attenuation = albedo;

                vec3 outward_normal;
                vec3 reflected = reflect(ray_in.direction(), rec.normal);
                vec3 refracted;

                float ni_over_nt;
                float reflect_prob;
                float cosine;

    // Dealing with Ray Enter/Exit Object
    // When a ray shoots through an object back into the vacuum,
    // ni_over_nt is equal to the ref index, and the surface normal needs to be inverted
    if (dot(ray_in.direction(), rec.normal) > 0){
        outward_normal = -rec.normal;
        ni_over_nt = ref_index;
        cosine = dot(unit_vector(ray_in.direction()), rec.normal);
    }

    // Ray shoots into an object
    else {
        outward_normal = rec.normal;
        ni_over_nt = 1 / ref_index;
        cosine = -dot(unit_vector(ray_in.direction()), rec.normal);
    }


    
    // Dealing with Ray Reflection/Refraction (see Fresnel.gif)
    // refracted ray exists
    if(refract(ray_in.direction(), outward_normal, ni_over_nt, refracted)){
        reflect_prob = schlick(cosine, ref_index);
    }
    // refracted ray does not exist
    else{
        // total reflection
        reflect_prob = 1.0;
    }



    // Reflection and refraction occur for dielectric materials, but we can only choose one ray for 'scattered'.
    // Because we are multisampling each pixel, we can use the same idea to average the color between reflection and refraction.
    if(random_double(0.001, 0.999) < reflect_prob) { // when total reflection happens, reflect_prob is 1
        scattered = ray(rec.p, reflected);
    }
    else {
        scattered = ray(rec.p, refracted);
    }

        return true;
    }
    double ref_index;
    vec3 albedo;
};



#endif // !MATERIALH