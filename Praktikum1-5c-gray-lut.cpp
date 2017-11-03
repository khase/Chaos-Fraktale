
#include <windowRasterized.h>
#include <chrono>
#include <iostream>
#include <random>

const int ROWS = 512;
const int COLLUMNS = 512;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

cf::WindowRasterized window = cf::WindowRasterized(512, 512);
std::mt19937 generator = std::mt19937(std::random_device().operator()());

int intToGray(int i) {
	return (i ^ (i << 1));
}

int main() {
	window.show();

	// read LUT
	std::cout << "loading MULTCOL4.pal" << std::endl;
	std::vector<cf::Color> lut = cf::readPaletteFromFile("../../chaos_files/Multcol4.pal");

	// Draw
	auto t0 = Time::now();
	std::cout << "Drawing..." << std::endl;
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLLUMNS; c++) {
			if ((intToGray(c) & intToGray(r)) == 0) {
				window.setColor(r, c, lut.at(intToGray(c) & intToGray(r) % 255));
			}
		}
	}
	window.show();
	auto t1 = Time::now();
	fsec fs = t1 - t0;
	ms d = std::chrono::duration_cast<ms>(fs);
	std::cout << "Finished in " << d.count() << "ms" << std::endl;


	std::cout << "Press any key to close" << std::endl;
	window.waitKey();
	return 0;
}