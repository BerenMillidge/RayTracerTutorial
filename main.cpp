// okay. and now basic main function

#include <iostream>
#include <fstream>
#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hitable_list.hpp"
#include "float.h" // what is this weird float.h file!?
#include "camera.hpp"
using namespace std;

// okay, so just trace out a pattern... see if it works?
// this returns a color vector given a ray... i nthe simple case? of a blank background
// where it just gets the backgroudn colour?

// now add a sphere to the mix
bool hit_sphere(const vec3& center, float radius, const ray& r) {
	// this tells whether a ray extending long the path hits the sphere
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction()); // a in the quadratic equation
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b*b - 4*a*c;
	//return (discriminant > 0); // i.e. if there is asolution it solves this quadratic equation
	// we don't need to know the actual solution 
	// visualize the surface normals
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		// solve the quardatic equation
		return (-b - sqrt(discriminant)) / (2.0*a);
	}
}



vec3 color(const ray& r) {
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5*(unit_direction.y() + 1.0); // whatever this does is entirely confusing to me
	// is this the one that greates the colour gradient, so it is ultimately arbitrary
	// I think so 
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t*vec3(0.5,0.7,1.0);
	// so it effectievly just scales white/blue depending on the up/downness of the y coordinate
	// it then scales 0<t<1 so t=1 = blue and t = 0 = white, and scales lienarly between them!
}

vec3 color_with_sphere(const ray& r){
	if (hit_sphere(vec3(0,0,-1), 0.5, r)){
		return vec3(1,0,0); //the colour of the sphere is just one
	}
	// the previous linear matching of the colors
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5*(unit_direction.y() + 1.0); // whatever this does is entirely confusing to me
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t*vec3(0.5,0.7,1.0);

}

vec3 color_surface_normals(const ray& r){
	float t = hit_sphere(vec3(0,0,-1), 0.5, r); // this is the time the vector hits the sphere, if any
	// if it doesn't hit its -1
	if (t > 0.0) {
		// i.e there is a oslution - compute the surface normal
		vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0,0,-1)); // so sphere is at center 0,0,1 so it subtractsthe center of the sphere from the point!
		// normalize the vector to be between 0 and 1
		return 0.5*vec3(N.x()+1, N.y()+1, N.z()+1);
	}
	vec3 unit_direction = unit_vector(r.direction());
	t = 0.5*(unit_direction.y() + 1.0); // whatever this does is entirely confusing to me
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t*vec3(0.5,0.7,1.0);
}

// oh my god!!! it's managed to render a sphere... that's just crayz... it just solevs the quadratic and fixes it
// that's fantastic!

// color hitable list surface
vec3 color_hitable_world(const ray& r, hitable *world) {
	// just a pointer to the hitable while the ray is dereferenced?
	float MAXFLOAT = 1e8;
	hit_record rec;// initialize empty hit record
	if (world -> hit(r, 0.0, MAXFLOAT, rec)) {
		// return the surface normal vector!
		return 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0,1.0) + t*vec3(0.5,0.7,1.0);
	}
}


int main_old() {
	// start with simplest raytracer as bfore!
	int nx = 200;
	int ny = 100;
	ofstream myfile;
	myfile.open("surface_normals.txt");
	myfile << "P3\n" << nx << " " << ny << "\n255\n";
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0,0.0);
	vec3 vertical(0.0, 2.0,0.0);
	vec3 origin(0.0,0.0,0.0);
	for (int j = ny-1; j>=0; j--) {
		for (int i = 0; i < nx; i++){
			// so it iterates through all the pixels on the screen
			// then it calculates the u,v position of the vetor to the pixel
			// then uses the simple colour functoin above to calculate the simulated colour
			float u = float(i)/ float(nx);
			float v = float(j) / float(ny);

			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			vec3 col = color_surface_normals(r);
		

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			myfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	myfile.close();
	return 0;
}

int main_without_camera() {
	int nx = 200;
	int ny = 100;
	ofstream myfile;
	myfile.open("hitable_world.txt");
	myfile << "P3\n" << nx << " " << ny << "\n255\n";
	hitable *list[2]; // create a new hitable list
	list[0] = new sphere(vec3(0,0,-1), 0.5);
	list[1] = new sphere(vec3(0,-100.5,-1), 100);
	hitable *world = new hitable_list(list, 2);
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0,0.0);
	vec3 vertical(0.0, 2.0,0.0);
	vec3 origin(0.0,0.0,0.0);
	for (int j = ny-1; j>=0; j--){
		for (int i = 0; i<nx; i++){
			float u = float(i)/ float(nx);
			float v = float(j) / float(ny);

			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			vec3 col = color_hitable_world(r, world);

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			myfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	myfile.close();
	return 0;
}
// oh my god... that actually worked... that's crazy!

int main_with_camera() {
	int nx = 200;
	int ny = 100;
	ofstream myfile;
	myfile.open("camera_samples.txt");
	myfile << "P3\n" << nx << " " << ny << "\n255\n";
	hitable *list[2]; // create a new hitable list
	list[0] = new sphere(vec3(0,0,-1), 0.5);
	list[1] = new sphere(vec3(0,-100.5,-1), 100);
	hitable *world = new hitable_list(list, 2);
	camera cam;
	int num_samples = 100;
	for (int j = ny-1; j>=0; j--){
		for (int i = 0; i<nx; i++){
			// define starting color
			vec3 col(0,0,0);
			// now get a number of samples for each pixel
			for (int s = 0; s < num_samples; s++) {
				float u = float(i + drand48())/ float(nx);
				float v = float(j + drand48()) / float(ny);
				ray r = cam.get_ray(u,v);
				vec3 p = r.point_at_parameter(2.0); // why arewe calculating this here? it's not used?
				col += color_hitable_world(r, world);
			}
			col /= float(num_samples);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			myfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	myfile.close();
	return 0;
	// fantastically this works as well... this is amazing!

}

int main() {
	return main_with_camera();
}
