#ifndef MATERIALH
#define MATERIALH

#include "Hittable.h"
#include "Camera.h"


// Simulate reflectivity of a Metal surface (see MetalReflectivity.png)
Vec3 reflect(const Vec3& v, const Vec3& n){
    return v - 2*dot(v,n)*n; // v enters the Hittable, which is why subtraction is required.
}

// Simulate refraction of light through an object (See RefractiveIndex.png and SnellsLaw.png)
bool refract(const Vec3& v, const Vec3& n, double niOverNt, Vec3& refracted) {
    Vec3 uv = unitVector(v);
    double dt = dot(uv, n);
    double discriminant = 1.0 - niOverNt * niOverNt * (1-dt*dt);

    
    if(discriminant > 0) {
        refracted = niOverNt * (uv-n*dt) - n*sqrt(discriminant);
        return true;
    }
    else {
    // either the discriminant is negative, which represents internal reflection,
    // or it's zero, where the refracted ray is perpendicular to the normal.
        return false; 
    }
}

// Schlick's approximation of Fresnel Equations for partial reflectance 
float schlick(float cosine, float refractiveIndex) {
    float r0 = (1 - refractiveIndex) / (1 + refractiveIndex); // refractiveIndex = n2/n1
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class Material {
    public:
    virtual bool scatter(const Ray& rayIn, 
                        const HitRecord& rec, 
                        Vec3& attenuation,
                        Ray& scattered) const = 0;
};

// Matte surface
// Light that reflects off a diffuse surface has its direction randomized.
// Light may also be absorbed. See Diffuse.png for illustration and detailed description
class Lambertian : public Material {
    public:
        Lambertian(const Vec3& a) : albedo(a){};
        virtual bool scatter(const Ray& rayIn, 
                            const HitRecord& rec, 
                            Vec3& attenuation, 
                            Ray& scattered) const {
            Vec3 scatterDirection = rec.p + rec.normal + randomUnitVector();
            scattered = ray(rec.p, scatterDirection - rec.p, rayIn.moment());
            attenuation = albedo;
            return true;
        }
    Vec3 albedo; // reflectivity

};

// Simulate reflection of a Metal (see MetalReflectivity.png)
// See FuzzyReflections.png for a visualization of fuzziness.
class Metal : public Material {
    public:
        Metal(const Vec3& a, double f) : albedo(a) {
            if (f<1) fuzz = f; else fuzz = 1; // max fuzz of 1, for now.
        }
        virtual bool scatter(const Ray& rayIn, 
                            const HitRecord& rec, 
                            Vec3& attenuation, 
                            Ray& scattered) const {
        Vec3 reflected = reflect(unitVector(rayIn.direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz*randomUnitSphereCoordinate(), rayIn.moment()); // large spheres or grazing rays may go below the surface. In that case, they'll just be absorbed.
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0.0;
    }

    Vec3 albedo;
    double fuzz;
};

class Dielectric : public Material {
    public:
        Dielectric(Vec3 a, double ri) : albedo(a), refractiveIndex(ri) {}

        virtual bool scatter(
            const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered
        ) const {

            attenuation = albedo;

            double n1OverN2 = (rec.frontFace) ? (1.0 / refractiveIndex) : (refractiveIndex);

            Vec3 unitDirection = unitVector(rayIn.direction());
            
            double cosine = fmin(dot(-unitDirection, rec.normal), 1.0);
            double reflectRandom = randomDouble(0,1);
            double reflectProbability;

            Vec3 refracted;
            Vec3 reflected;

            if (refract(unitDirection, rec.normal, n1OverN2, refracted)) {
                reflectProbability = schlick(cosine, refractiveIndex);

                if (reflectRandom < reflectProbability) {
                    Vec3 reflected = reflect(unitDirection, rec.normal);
                    scattered = Ray(rec.p, reflected, rayIn.moment());
                    return true;
                }
                scattered = Ray(rec.p, refracted, rayIn.moment());
                return true;
            }

            else {
                reflected = reflect(unitDirection, rec.normal);
                scattered = Ray(rec.p, reflected, rayIn.moment());
                return true;
            }

            
        
        }
    public:
        double refractiveIndex;
        Vec3 albedo;
};



#endif // !MATERIALH