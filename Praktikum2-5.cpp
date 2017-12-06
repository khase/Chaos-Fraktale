#include "IFS.h"
#include "windowVectorized.h"
#include <iostream>
#include <fstream>
#include <random>
#include <string>

int main(int numArgs, char** Args) {
	cf::IteratedFunctionSystem ifs;
	ifs.read("../../chaos_files/INVERS.IFS");
	const std::string align = " :  ";
	std::cout << "Name" << align << ifs.getName() << '\n'
		<< "Num transformation" << align << ifs.getNumTransformations() << '\n'
		<< "Interval X min" << align << ifs.getRangeX().min << '\n'
		<< "Interval X max" << align << ifs.getRangeX().max << '\n'
		<< "Interval Y min" << align << ifs.getRangeY().min << '\n'
		<< "Interval Y max" << align << ifs.getRangeY().max << '\n'
		<< "\n\n"
		<< std::endl;

	for (std::size_t i = 0; i < ifs.getNumTransformations(); ++i)
		std::cout << "Transformation " << i << ": \n" << ifs.getTransformation(i) << std::endl;

	cf::WindowVectorized window(512, ifs.getRangeX(), ifs.getRangeY(), "Praktikum 2", cf::Color::ORANGE);
	std::cout << "Created window size:\n"
		<< "Width : " << window.getWidth() << std::endl
		<< "Height: " << window.getHeight() << std::endl;
	//window.drawAxis();
	window.show();

	for (int x = 0; x < 512; x++) {
		for (int y = 0; y < 512; y++) {
			cf::Point p = cf::Point(x, y);
			cf::Point intervall = window.transformPoint_fromImage_toInterval(p);

			int div = -1;
			for (int i = 0; i < 15; i++) {
				int transformIndex = 2;
				if (intervall.x <= 0.5 && intervall.y <= 0.5) {
					transformIndex = 0;
				}
				else if (intervall.x > 0.5 && intervall.y <= 0.5) {
					transformIndex = 1;
				}
				glm::mat3x3 transform = ifs.getTransformation(transformIndex);
				glm::vec3 v(intervall.x, intervall.y, 1);
				v = transform * v;

				cf::Point pNext = cf::Point(v.x, v.y);
				if (std::pow(pNext.x, 2) + std::pow(pNext.y, 2) > 10000) {
					div = i;
					break;
				}
				intervall = pNext;
			}
			try {
				cf::Point wahh = window.transformPoint_fromImage_toInterval(p);
				if (div == -1) {
					window.setColor(wahh.x, wahh.y, cf::Color().BLACK);
				}
				else {
					window.setColor(wahh.x, wahh.y, cf::Color(255 - (div * 100), 255 - (div * 100), 255 - (div * 100)));
					//window.setColor(wahh.x, wahh.y, cf::Color().WHITE);
				}
			}
			catch (...) {

			}
		}
		window.show();
	}

	std::cout << "Press any key to close Window" << std::endl;
	window.waitKey();
	return 0;
}