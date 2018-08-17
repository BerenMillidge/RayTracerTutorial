// so this is defining an abstract class for any object or list of objects we might want to hit with a ray!
// only has one major method - hit 

#ifndef HITABLEH
#define HITABLEH

#include "ray.hpp"

struct hit_record {
	float t;
	vec3 p;
	vec3 normal; 
};
// this just stores all the information about the hit that we need for the moment

class hitable {
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record &rec) const = 0; // what does this const = 0 do?
};

#endif