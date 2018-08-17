// okay, so the core of a raytracer is that it shoots out rays from a camera into the scene... it sees what
// each ray intersects. at each intersection it generates a colour, and that colour is the pixel
// more complicated raytracers will include reflections, scattering and so forth!

// let's define a simple ray class which simply moves along a line in time
#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

class ray {
public:
	vec3 _origin;
	vec3 _direction;
	ray() {};
	ray(const vec3 &origin, const vec3& direction) : _origin(origin), _direction(direction) {}; // not sure how this construtor works?
	vec3 origin() const { return _origin; }
	vec3 direction() const { return _direction; }
	vec3 point_at_parameter(float t) const {
		return _origin + _direction * t; // this is just the ray propagating along the line at time t
	}
};

// the ray just traverses forward across time in its direction which shows where it is

#endif