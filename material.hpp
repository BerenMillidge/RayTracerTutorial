// okay, so this now defines the material within some kind of abstract class... whic his fantastic
// there's so muchto do in a raytracer... which is just great1

#ifndef HITABLEH
#define HITABLEH

#include "ray.hpp"
#include "hitable.hpp"

class material {
public: 
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray&scattered) const = 0;

};

class lambertian : public material {
public:
	// a perfect scatere/diffuse matte object is caled a lambertian
	lambertian(const vec3& a) : albedo(a) {};
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray&scattered) const {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		// this is the target pint- including the point + the normal then a random in unit sphere?
		scattered = ray(rec.p, target-rec.p);
		attenuation = albedo; // well thats super useful?
		return true;
	}
	vec3 albedo; // not totally sure how it totally works but who knows?
	// a lambertian has random scattering, but what about a meta/?
};

vec3 perfect_reflect(const vec3& v, const vec3&n) {
	return v - 2*dot(v,n)*n; // this is the formula for a perfect reflection around the surface normal
}

class metal : public material {
public:
	metal(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 reflected = perfect_reflect(unit_vector(r.in_direction()), rec.normal);
		scattered = ray(rec.p, reflected); // this is the origin and the new direction
		attenation =albedo;// not sure what's going o nthere
		return (dot(scattered.direction(), rec.normal) > 0);
		// so if the directio nand the normal is greater than zero return true?
		// not sure why it is doing this?
	}
	vec3 albedo;
};
#endif