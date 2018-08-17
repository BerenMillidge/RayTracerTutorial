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

// somehow doing this has just exploded everything... and I have no idea why or what? it's so confusing to me...
// what am I doing wrong... why has all of this caused the compilerto explode when it worked before?
// even with the hitable object list?

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

// now give things ideally matte surfaces... i.e. they reflect the ray at each bounde i na completely random direction
// thye are not perfect absorbers... but rather reflect each ray in some random direction
// to do this create the randomcircle and then send out another ray frmo the hitpoint in a rando mdirection
// and then add what it hits too! // it is essentially the exact inverse of whatthe eye see.s. it's so cool to go and figure out
// what is happening here, and itm akes perfect sense

// now a diffuse scattering color functino
vec3 color_hitable_world_matte_surface(const ray& r, hitable *world){
	float MAXFLOAT = 1e8;
	hit_record rec;
	if (world->hit(r,0.0, MAXFLOAT, rec)) {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere(); // creates new target vector
		// so the p is the hitpoint, normal is normal right?
		// the nrecursively calls this functoin to calculate the next bound!
		return 0.5*color_hitable_world_matte_surface(ray(rec.p, target-rec.p), world);

	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0,1.0) + t*vec3(0.5,0.7,1.0);
	}
}
const float MAXFLOAT = 1e8;
const int DEPTHLIMIT = 50;

vec3 color_with_material(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world ->hit(r, 0.01, MAXFLOAT, rec)){
		ray scattered;
		vec3 attenuation;
		if (depth < DEPTHLIMIT && rec.mat_ptr->scatter(r,rec,attenuation, scattered)) {
			return attenuation * color_with_material(scattered, world, depth+1);
			// recursively invoke the function to calculate reflections around
		}
		else {
			return vec3(0,0,0); // if greater than 50 reflections return nothing!
		}
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
	myfile.open("matte_surface.txt");
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
			//vec3 col = color_hitable_world(r, world);
			vec3 col = color_hitable_world_matte_surface(r, world);

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
	myfile.open("matte_surface_gamma_corrected.txt");
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
				col += color_hitable_world_matte_surface(r, world);
			}
			col /= float(num_samples);
			// to gamma correct
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			myfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	// oh my god! it worked... and it automatically created shadowing... that's just amazing!
	// see there's nothing really compliacted about this and I thought that it was some super arcane stuff
	// it seems like that's a lesson I need to learn over and over again... just because you don't know smoething doens't mean
	// it is this supercomplicated and scaty and arcane subject... it'susually quite simple and interesting!
	myfile.close();
	return 0;
	// fantastically this works as well... this is amazing!

}

int main_with_metal() {
	int nx = 200;
	int ny = 100;
	ofstream myfile;
	myfile.open("material_test.txt");
	myfile << "P3\n" << nx << " " << ny << "\n255\n";
	hitable *list[4]; // create a new hitable list
	list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.8,0.3,0.3)));
	list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8,0.8,0.0)));
	list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8,0.6,0.2)));
	list[3] = new sphere(vec3(-1,0,-1), 0.5, new metal(vec3(0.8,0.8,0.8)));
	hitable *world = new hitable_list(list, 2);
	camera cam;
	int num_samples = 100;
	for (int j = ny-1; j>=0; j--){
		for (int i = 0; i<nx; i++){
			vec3 col(0,0,0);
			for (int s = 0; s < num_samples; s++) {
				float u = float(i + drand48())/ float(nx);
				float v = float(j + drand48()) / float(ny);
				ray r = cam.get_ray(u,v);
				vec3 p = r.point_at_parameter(2.0); // why arewe calculating this here? it's not used?
				col += color_with_material(r, world,0);
			}
			col /= float(num_samples);
			// to gamma correct
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			myfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	myfile.close();
	return 0;

}

int main() {
	//return main_with_metal();
	return main_with_metal();
	// I'm not sure what's going on here but I'm getting segfaults!
}
