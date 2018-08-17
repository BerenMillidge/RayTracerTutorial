// this defines a sphere as extending hittable and is thus a hittable object
// using proper OOP paradigms

#ifndef SPHEREH
#define SPHEREH

#include "hitable.hpp"

class sphere: public hitable { //extends hitable
public:
	sphere() {}
	sphere(vec3 cen, float r, material* mat): center(cen), radius(r), _material(mat) {};
	sphere(vec3 cen, float r): center(cen), radius(r)  {}; // not sure how these type definitions work
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	vec3 center;
	float radius;
	material* _material;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0) {
		// get solution using quadratic formula
		float temp = (-b - sqrt(b*b - a*c))/ a;
		// temp is the solution to the quadratic equation... check if within boudns.. it only seems to get one solution
		// probably just ignores the negative solution
		if (temp < t_max && temp > t_min){
			// if temp within bounds create hitable record!
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t); // this is the point of intersectino, it's a bit confusing the 3vectors are being used for locations and directions
			rec.normal = (rec.p - center) / radius;
			return true;
		}
		// if not then calculate the other root
		temp = (-b + sqrt(b*b - a*c)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false; // if no solution obviously
}
#endif