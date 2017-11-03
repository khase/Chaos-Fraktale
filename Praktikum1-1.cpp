
#include <windowRasterized.h>
#include <iostream>
#include <random>

int main() {
	cf::WindowRasterized window = cf::WindowRasterized(512, 512);
	std::mt19937 generator = std::mt19937(std::random_device().operator()());
	std::uniform_int_distribution<int> random(0, 2);
	window.show();

	cf::Point input[3];
	for (int i = 0; i < 3; i++) {
		input[i] = window.waitMouseInput();
		window.drawCircle(input[i], 2, 2, cf::Color().GREEN);
		window.show();
		std::cout << "X: " << input[i].x  << " Y: " << input[i].y << std::endl;
	}

	cf::Point p = window.waitMouseInput();
	window.drawCircle(p, 2, 2, cf::Color().RED);
	window.show();
	std::cout << "X: " << p.x << " Y: " << p.y << std::endl;

	// Iterationsschleife
	cf::Color col = cf::Color().RandomColor();
	for (int i = 0; i < 100000; i++) {
		cf::Point theChoosenOne = input[random(generator)];
		cf::Point pNeu = (theChoosenOne + p) / 2;

		window.setColor(pNeu.x, pNeu.y, col);

		p = pNeu;

		if (i % 1000 == 0) {
			window.show();
			col = cf::Color().RandomColor();
		}
	}

	std::cout << "Press any key to close" << std::endl;
	window.waitKey();
	return 0;
}