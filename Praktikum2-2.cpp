#include "IFS.h"
#include "windowVectorized.h"
#include <iostream>
#include <fstream>
#include <random>
#include <string>

int main(int numArgs, char** Args) {
	std::string filePath;
	if (numArgs <= 6) {
		std::cout << "Please provide a .ifs file, if you want a different ifs file";
		//filePath = "../../chaos_files/Sierpinski.ifs";
		//filePath = "../../chaos_files/DRACHENFLAECHE_4.IFS";
		//filePath = "../../chaos_files/DUERER_5_ECK.IFS";
		//filePath = "../../chaos_files/KRISTALL_1.IFS";
		//filePath = "../../chaos_files/MENGER_TEPPICH.IFS";
		//filePath = "../../chaos_files/SIERP_Verwandter_2.IFS";
		filePath = "../../chaos_files/FARN_1.IFS";
	}
	else {
		filePath = Args[0];
	}

	std::cout << "open ifs: " << filePath << std::endl;
	// create IFS object
	// and read *.ifs file
	cf::IteratedFunctionSystem ifs;
	ifs.read(filePath);

	// output all parsed data of IFS file
	const std::string align = " :  ";
	std::cout << "Name" << align << ifs.getName() << '\n'
		<< "Num transformation" << align << ifs.getNumTransformations() << '\n'
		<< "Interval X min" << align << ifs.getRangeX().min << '\n'
		<< "Interval X max" << align << ifs.getRangeX().max << '\n'
		<< "Interval Y min" << align << ifs.getRangeY().min << '\n'
		<< "Interval Y max" << align << ifs.getRangeY().max << '\n'
		<< "\n\n"
		<< std::endl;

	std::vector<double> dets;
	for (std::size_t i = 0; i < ifs.getNumTransformations(); ++i) {
		glm::mat3x3 trans = ifs.getTransformation(i);
		std::cout << "Transformation " << i << ": \n" << trans;

		double det = std::abs(trans[0][0] * trans[1][1] - trans[0][1] * trans[1][0]);
		std::cout << "Determinante: " << det << std::endl << std::endl;
		dets.push_back(det);
	}

	double gesamt = 0;
	for (std::vector<double>::iterator it = dets.begin(); it != dets.end(); it++) {
		gesamt += (*it);
	}
	std::cout << "Gesamt: " << gesamt <<std::endl << std::endl;

	for (std::vector<double>::iterator it = dets.begin(); it != dets.end(); it++) {
		if (*it == 0) {
			*it = gesamt / (ifs.getNumTransformations() * 25);
			gesamt += (*it);
			std::cout << "Passe " << it - dets.begin() << " an: " << (*it) << std::endl;
		}
	}
	std::cout << std::endl;

	double tmp = 0;
	std::vector<double> wahrscheinlichkeiten;
	for (std::vector<double>::iterator it = dets.begin(); it != dets.end(); it++) {
		double wahrsch = (*it) / gesamt;
		wahrscheinlichkeiten.push_back(tmp + wahrsch);
		tmp += wahrsch;
	}

	cf::WindowVectorized window(512, ifs.getRangeX(), ifs.getRangeY(), "Praktikum 2", cf::Color::ORANGE);
	std::cout << "Created window size:\n"
		<< "Width : " << window.getWidth() << std::endl
		<< "Height: " << window.getHeight() << std::endl;
	//window.drawAxis();
	window.show();

	std::cout << "Please set a point on the window" << std::endl;
	cf::Point point1 = window.waitMouseInput();
	window.drawCircle(point1, 3, -1 /*fill circle*/, cf::Color::WHITE);
	window.show();

	//std::mt19937 generator = std::mt19937(std::random_device().operator()());
	//std::uniform_int_distribution<int> random(0, ifs.getNumTransformations() - 1);

	std::mt19937 generator = std::mt19937(std::random_device().operator()());
	std::uniform_real_distribution<double> random(0, 1);

	glm::vec3 v(point1.x, point1.y, 1);
	cf::Color col = cf::Color().RandomColor();
	for (int i = 0; i < 15000 * 2; i++) {
		double rand = random(generator);
		int auswahl = 0;
		for (std::vector<double>::iterator it = wahrscheinlichkeiten.begin(); it != wahrscheinlichkeiten.end(); it++) {
			if (rand <= (*it)) {
				auswahl = it - wahrscheinlichkeiten.begin();
				break;
			}
		}

		const glm::mat3x3& m = ifs.getTransformation(auswahl);
		v = m * v;
		//std::cout << "Vector after  transformation: " << v << std::endl << std::endl;
		window.setColor(v.x, v.y, col);

		if (i % 100 == 0) {
			window.show();
			col = cf::Color().RandomColor();
		}
	}

	std::cout << "Press any key to close Window" << std::endl;
	window.waitKey();
	return 0;
}

