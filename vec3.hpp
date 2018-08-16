// okay, for some reason we're definingthe vector in the header file, but who knows

#ifndef VEC3H
#define VEC3H

#include<math.h>
#include <stdlib.h>
#include<iostream>

class vec3 {

public:
	// these just define variou sconstructor methods on the vector class
	vec3() {}
	vec3(float e0, float e1, float e2) { e[0] = e0; e[1]=e1; e[2] = e2;}
	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const vec3& operator+() const { return *this; } // I have literally no idea what this does?
	inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) const { return e[i]; } // shouldn't this return the pointer?

	
  	inline vec3& operator+=(const vec3 &v2);
  	inline vec3& operator-=(const vec3 &v2);
  	inline vec3& operator*=(const vec3 &v2);
  	inline vec3& operator/=(const vec3 &v2);
 	inline vec3& operator*=(float t);
	inline vec3& operator/=(float t);

	// defines some more basic functions
	inline float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
	inline float squared_length() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]}
	inline void make_unit_vector(); // why is thie not a normal function

	float e[3]; // what does this do?v2)

	// okay define other vector addition multiplication and so forth

	inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
		return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2]+v2.e[2]);
	}
	// the ands are presumably dereferened objects since the vectors are heap allocated structs?
	inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
		return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
	}
	inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
		return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
	}
	inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
		return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
	}
	// this operato roverloading is nice but a whole lot simpler in julia!
	// and some geometric functions
	inline float dot(const vec3 v1, const vec3 & v2){
		return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
	}

	inline vec3 cross(const vec3 ^v1, const vec3 & v2) {
		return vec3 ( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
					(-v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0]),
					(v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]));
	}
	inline vec3 unit_vecto(vec3 v) {
		return v / v.length();
	}