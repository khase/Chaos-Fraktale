
#include <windowRasterized.h>
#include <chrono>
#include <iostream>
#include <random>

const int ROWS = 512;
const int COLLUMNS = 513;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

cf::WindowRasterized window = cf::WindowRasterized(512, 512);
std::mt19937 generator = std::mt19937(std::random_device().operator()());

int buffer[ROWS][COLLUMNS];

int main() {
	window.show();

	// init buffer (empty)
	std::cout << "Init buffer (empty)" << std::endl;
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLLUMNS; c++) {
			buffer[r][c] = 0;
		}
	}

	// set init value
	std::cout << "Init buffer (starting value)" << std::endl;
	buffer[0][1] = 1;

	// calculate matrix
	auto t0 = Time::now();
	std::cout << "Calculate matrix" << std::endl;
	for (int r = 1; r < ROWS; r++) {
		for (int c = 1; c < COLLUMNS; c++) {
			buffer[r][c] = (buffer[r-1][c] + buffer[r-1][c-1]) % 10;
		}
	}
	auto t1 = Time::now();
	fsec fs = t1 - t0;
	ms d = std::chrono::duration_cast<ms>(fs);
	std::cout << "Finished in " << d.count() << "ms" << std::endl;

	// map matrix to window
	t0 = Time::now();
	std::cout << "Map matrix to window" << std::endl;
	for (int r = 0; r < ROWS; r++) {
		for (int c = 1; c < COLLUMNS; c++) {
			if (buffer[c][r] != 0) {
				window.setColor(r, c - 1, cf::Color().RED);
			}
		}
	}
	window.show();
	t1 = Time::now();
	fs = t1 - t0;
	d = std::chrono::duration_cast<ms>(fs);
	std::cout << "Finished in " << d.count() << "ms" << std::endl;


	std::cout << "Press any key to close" << std::endl;
	window.waitKey();
	return 0;
}