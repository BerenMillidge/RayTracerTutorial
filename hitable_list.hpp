// okay, and define a list of hitable objects... would probably be moresimple to have this as the base class
// and then just create a hitable object as a hitable list of length 1, would be a tiny bit of overhead though
// who knows if relevant

#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.hpp"

class hitable_list: public hitable {
	// extends hitable obviously
public:
	hitable_list() {}
	hitable_list(hitable **l, int n) {list = l; list_size=n;} // I'm really not suer how these constructors work
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	hitable **list; // double pointers... exciting!
	int list_size;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	hit_record temp_rec; // create an empty hit record
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (int i = 0; i < list_size; i++){
		// iterate over list 
		if (list[i]->hit(r,t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
	// so it essentially finds the closest hit in existence in time, as it's the first one
	// no point in calculating the other hits relaly!
	// although it does calculate surface normals pointlessly for the other hits,
	// which is a bit inefficient but it doesn't really matter!

}
#endif