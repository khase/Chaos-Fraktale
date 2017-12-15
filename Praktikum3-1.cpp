#include "LSystem.h"
#include "windowVectorized.h"

using namespace std;

glm::mat3x3 getRotationMatrix(double grad) {
	glm::mat3x3 m = {	cos(grad * (3.141593 / 180.0)),		sin(grad * (3.141593 / 180.0)),		0,
						-sin(grad * (3.141593 / 180.0)),	cos(grad * (3.141593 / 180.0)),		0,
						0,									0,									1 };
	return m;
}

int main(int argc, char** argv) {
	// receive file name/path
	string filePath;
	if (argc < 2) {
		std::cout << "Please provide a .lin file, if you want a different file\n\n\n";
		filePath = "../../chaos_files/";
		filePath += "DRACHEN_KURVE_3.lin";
	}
	else
		filePath = argv[1];

	// create and parse *.lin file
	cf::LindenmayerSystem ls; // alternative:     cf::LSystem ls;
	ls.read(filePath);

	// print all data found in *.lin file
	string align = " :  ";
	cout << "Name" << align << ls.getName() << '\n'
		<< "Axiom" << align << ls.getAxiom() << '\n'
		<< "Number of productions" << align << ls.getNumProductions() << '\n'
		<< "Clear window each time?" << align << (ls.clearWindowEachTime() ? "yes" : "no") << '\n'
		<< "Start angle" << align << ls.getStartAngle() << '\n'
		<< "Adjustment angle" << align << ls.getAdjustmentAngle() << '\n'
		<< "Scale" << align << ls.getScale() << '\n'
		<< "Interval X" << align << ls.getRangeX() << '\n'
		<< "Interval Y" << align << ls.getRangeY() << '\n'
		<< endl;

	cout << "All productions:" << endl << endl;
	for (const auto& e : ls.getAllProductions()) {
		cout << "Symbol: " << e.first << "\nProduction: " << e.second << endl << endl;
	}

	cf::WindowVectorized window(300, ls.getRangeX(), ls.getRangeY(), "LSystem", cf::Color::ORANGE);
	
	for (int depth = 1; depth <= 20; depth++) {
		if (ls.clearWindowEachTime()) {
			window.clear(cf::Color::ORANGE);
		}
		cf::Color lineColor = cf::Color().RandomColor();
		cf::Point cur = cf::Point(0, 0);
		cf::Point dir = cf::Point(1, 0) * pow(ls.getScale(), depth); 
		glm::vec3 v(dir.x, dir.y, 1);
		glm::mat3x3 m = getRotationMatrix(ls.getStartAngle());
		v = m * v;
		dir = cf::Point(v.x, v.y);
		const cf::LSystem_Controller con(depth, ls);
		int i = 0;
		vector<cf::Point> posStack;
		vector<cf::Point> dirStack;
		for (const auto& e : con) {
			if ((e >= 'a' && e <= 'z') || (e >= 'A' && e <= 'Z')) {
				window.drawLine(cur, cur + dir, 1, lineColor);
				cur = cur + dir;
				i++;
				if (i % (int)(1 / pow(ls.getScale(), depth + 1)) == 0) {
					window.show();
				}
			}
			else if (e == '+') {
				glm::vec3 v(dir.x, dir.y, 1);
				glm::mat3x3 m = getRotationMatrix(ls.getAdjustmentAngle());
				v = m * v;
				dir = cf::Point(v.x, v.y);
			}
			else if (e == '-') {
				glm::vec3 v(dir.x, dir.y, 1);
				glm::mat3x3 m = getRotationMatrix(-ls.getAdjustmentAngle());
				v = m * v;
				dir = cf::Point(v.x, v.y);
			}
			else if (e == '[') {
				posStack.push_back(cur);
				dirStack.push_back(dir);
			}
			else if (e == ']') {
				cur = posStack.back();
				posStack.pop_back();
				dir = dirStack.back();
				dirStack.pop_back();
			}
			else {
				cout << "no rule for '" << e << "'" << endl;
			}
		}
		Sleep(1000);
	}

	window.show();
	window.waitKey();
	return 0;
}