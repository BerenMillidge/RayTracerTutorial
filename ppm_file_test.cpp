// okay, just testing that outputting images as ppm file works
// and to get used to a bit of cpp syntax

#include <iostream>
#include <fstream>
using namespace std;

int main() {
	int nx = 200;
	int ny = 100;
	ofstream myfile;
	myfile.open("ppm_test.txt");
	myfile << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny-1; j>=0; j--) {
		for (int i = 0; i < nx; i++){
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.2;
			int ir = int(255.99*r);
			int ig = int(255.99*g);
			int ib = int(255.99*b);
			myfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	myfile.close();
	return 0;
}
// oh wow, with a bit of fiddling... it works!