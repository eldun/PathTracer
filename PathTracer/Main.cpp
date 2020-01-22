#include <iostream>
#include "Vec3.h"

int main() {
	int nx = 200; // Number of horizontal pixels
	int ny = 100; // Number of vertical pixels
	std::cout << "P3\n" << nx << " " << ny << "\n255\n"; // P3 signifies ASCII, 255 signifies max color value
	for (int j = ny - 1; j >= 0; j--) { // RGB triplets
		for (int i = 0; i < nx; i++) {
			vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}