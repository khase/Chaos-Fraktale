
#include <windowRasterized.h>
#include <iostream>
#include <random>

int main() {
	cf::WindowRasterized window = cf::WindowRasterized(512, 512);
	std::mt19937 generator = std::mt19937(std::random_device().operator()());
	std::uniform_int_distribution<int> random(0, 7);
	window.show();

	cf::Point input[8];
	for (int i = 0; i < 4; i++) {
		input[i] = window.waitMouseInput();
		window.drawCircle(input[i], 2, 2, cf::Color().GREEN);
		window.show();
		std::cout << "X: " << input[i].x << " Y: " << input[i].y << std::endl;
	}

	input[4] = (input[0] + input[1]) / 2;
	input[5] = (input[1] + input[2]) / 2;
	input[6] = (input[2] + input[3]) / 2;
	input[7] = (input[3] + input[0]) / 2;

	window.drawCircle(input[4], 2, 2, cf::Color().BLUE);
	window.drawCircle(input[5], 2, 2, cf::Color().BLUE);
	window.drawCircle(input[6], 2, 2, cf::Color().BLUE);
	window.drawCircle(input[7], 2, 2, cf::Color().BLUE);
	window.show();

	cf::Point p = window.waitMouseInput();
	window.drawCircle(p, 2, 2, cf::Color().RED);
	window.show();
	std::cout << "X: " << p.x << " Y: " << p.y << std::endl;

	// Iterationsschleife
	cf::Color col = cf::Color().RandomColor();
	for (int i = 0; i < 500000; i++) {
		cf::Point theChoosenOne = input[random(generator)];

		cf::Point pDist = p - theChoosenOne;
		pDist = pDist / 3;
		cf::Point pNeu = theChoosenOne + pDist;

		window.setColor(pNeu.x, pNeu.y, col);

		p = pNeu;

		if (i % 5000 == 0) {
			window.show();
			col = cf::Color().RandomColor();
		}
	}

	std::cout << "Press any key to close" << std::endl;
	window.waitKey();
	return 0;
}