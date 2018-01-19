#include "LSystem.h"
#include "utils.h"
#include "window3D.h"

using namespace std;

glm::mat4x4 getRotationMatrixZ(double grad) {
	double rad = grad * (3.141593 / 180.0);
	glm::mat4x4 m = { 
		cos(rad),		sin(rad),	0,		0,
		-sin(rad),		cos(rad),	0,		0,
		0,				0,			1,		0,
		0,				0, 			0, 		1 };
	return m;
}

glm::mat4x4 getRotationMatrixY(double grad) {
	double rad = grad * (3.141593 / 180.0);
	glm::mat4x4 m = { 
		cos(rad),	0,	sin(rad),	0,
		0,			1,	0,			0,
		-sin(rad),	0,	cos(rad),	0,
		0, 			0,	0,			1 };
	return m;
}

glm::mat4x4 getRotationMatrixX(double grad) {
	double rad = grad * (3.141593 / 180.0);
	glm::mat4x4 m = { 
		1,	0,			0,			0,
		0,	cos(rad),	sin(rad),	0,
		0,	-sin(rad),	cos(rad),	0,
		0, 	0, 			0,			1 };
	return m;
}

class MyWindow : public cf::Window3D {
private:
	cf::LindenmayerSystem ls;

	int depth;
	int debug;
public:
	MyWindow(int* argc, char** argv, string filePath) : cf::Window3D(argc, argv, 800, 600, "Example") {
		this->m_AngleAdjustment = 2.0f;  // speed up  rotation
		this->m_CameraAdjustment = 0.1f; // slow down camera movement

	// create and parse *.lin file
		this->ls.read(filePath);

		this->depth = 1;
		this->debug = 1;

		// print all data found in *.lin file
		string align = " :  ";
		cout << "Name" << align << this->ls.getName() << '\n'
			<< "Axiom" << align << this->ls.getAxiom() << '\n'
			<< "Number of productions" << align << this->ls.getNumProductions() << '\n'
			<< "Clear window each time?" << align << (this->ls.clearWindowEachTime() ? "yes" : "no") << '\n'
			<< "Start angle" << align << this->ls.getStartAngle() << '\n'
			<< "Adjustment angle" << align << this->ls.getAdjustmentAngle() << '\n'
			<< "Scale" << align << this->ls.getScale() << '\n'
			<< "Interval X" << align << this->ls.getRangeX() << '\n'
			<< "Interval Y" << align << this->ls.getRangeY() << '\n'
			<< endl;

		cout << "All productions:" << endl << endl;
		for (const auto& e : this->ls.getAllProductions()) {
			cout << "Symbol: " << e.first << "\nProduction: " << e.second << endl << endl;
		}
	}
	virtual ~MyWindow() = default;
	virtual void draw() override {
		// clear all and redraw axis
		this->clear();
		this->drawAxis(2.f);

		cf::Color lineColor = cf::Color().RandomColor();
		glm::vec4 cur = glm::vec4(0, 0, 0, 0);
		glm::vec4 dir = glm::vec4(1, 0, 0, 0) * pow(this->ls.getScale(), depth);
		glm::vec3 v(dir.x, dir.y, 1);
		glm::mat4x4 trans = getRotationMatrixZ(0);// this->ls.getStartAngle());
		//dir = m * dir;

		const cf::LSystem_Controller con(depth, this->ls);
		int i = 0;
		vector<glm::vec4> posStack;
		vector<glm::vec4> dirStack;
		for (const auto& e : con) {
			if ((e >= 'a' && e <= 'z') || (e >= 'A' && e < 'X')) {
				if (e >= 'A' && e <= 'Z') {
					this->drawCylinder(trans * dir, cur, ((i == debug) ? 2 : 1) * 0.02, lineColor);
				}
				cur = cur + (trans * dir);
				i++;
				std::cout << i << "/" << debug << endl;
				if (i > debug) {
					break;
				}
			}
			else if (e == '+') {
				glm::mat4x4 m = getRotationMatrixZ(this->ls.getAdjustmentAngle());
				dir = m * dir;
			}
			else if (e == '-') {
				glm::mat4x4 m = getRotationMatrixZ(-this->ls.getAdjustmentAngle());
				dir = m * dir;
			}
			else if (e == '&') {
				glm::mat4x4 m = getRotationMatrixY(this->ls.getAdjustmentAngle());
				dir = m * dir;
			}
			else if (e == '^') {
				glm::mat4x4 m = getRotationMatrixY(-this->ls.getAdjustmentAngle());
				dir = m * dir;
			}
			else if (e == '*') {
				glm::mat4x4 m = getRotationMatrixX(this->ls.getAdjustmentAngle());
				dir = m * dir;
			}
			else if (e == '/') {
				glm::mat4x4 m = getRotationMatrixX(-this->ls.getAdjustmentAngle());
				dir = m * dir;
			}
			else if (e == '|') {
				glm::mat4x4 m = getRotationMatrixZ(180);
				dir = m * dir;
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
			else if ((e >= 'X' && e <= 'Z')) {
				// platzhalter
			}
			else {
				cout << "no rule for '" << e << "'" << endl;
			}
		}
	}

	void handleKeyboardInput(unsigned char key, int x, int y) override {
		printf("Key: %c pressed at mouse position: %d, %d\r", key, x, y);
		fflush(stdout);

		switch (key) {
			// change cylinder positions
		case 'j':
			this->m_StartPos.x -= 0.1f;
			break;
		case 'l':
			this->m_StartPos.x += 0.1f;
			break;
		case 'k':
			this->m_StartPos.y -= 0.1f;
			break;
		case 'i':
			this->m_StartPos.y += 0.1f;
			break;

			// change cylinder thickness
		case 'n':
			this->m_Diameter *= 1.5f;
			this->debug++;
			break;
		case 'm':
			this->m_Diameter /= 1.5f;
			this->debug--;
			if (this->debug == 0) {
				this->debug = 1;
			}
			break;

			// change cylinder rotation
		case 'u':
			this->m_Angle -= 5.f;
			break;
		case 'o':
			this->m_Angle += 5.f;
			break;

		case '+':
			this->depth++;
			break;
		case '-':
			this->depth--;
			if (this->depth == 0) {
				this->depth = 1;
			}
			break;
		default:
			break;
		}

		// change cylinder length
		if (key > '0' && key <= '9')
			this->m_Length = key - '0';
	}

	bool handleMousePressedMovement(MouseButton btn, int x, int y) override {
		std::string button = "N/A";
		switch (btn) {
		case MouseButton::CENTER:
			button = "middle";
			break;
		case MouseButton::LEFT:
			button = "left";
			break;
		case MouseButton::RIGHT:
			button = "right";
			break;
		default:
			break;
		}
		return false; // Set returnvalue to true to prevent camera movement
	}

	void handleMousePressEvent(MouseButton btn, MouseButtonEvent ev, int x, int y) override {
		std::string button = "N/A";
		std::string event = "N/A";
		switch (btn) {
		case MouseButton::CENTER:
			button = "middle";
			break;
		case MouseButton::LEFT:
			button = "left";
			break;
		case MouseButton::RIGHT:
			button = "right";
			break;
		case MouseButton::WHEEL_UP:
			button = "wheel up";
			break;
		case MouseButton::WHEEL_DOWN:
			button = "wheel down";
			break;
		default:
			break;
		}
		switch (ev) {
		case MouseButtonEvent::PRESSED:
			event = "pressed";
			break;
		case MouseButtonEvent::RELEASED:
			event = "released";
			break;
		default:
			break;
		}
	}

private:
	float m_Diameter = 0.1f;
	float m_Length = 1.f;
	float m_Angle = 0.f;
	glm::vec4 m_StartPos = glm::vec4(0.f, 0.f, 0.f, 0.f);
};

int main(int argc, char** argv) {
	// print camera usage
	MyWindow::printWindowUsage();

	// print object movement options
	std::cout << std::endl
		<< std::endl
		<< "Object movement:\n"
		<< "press:\ti/j/k/l\t to translate the object\n"
		<< "press:\tu/o    \t to   rotate  the object\n"
		<< "press: a number\t to    set    the objests length\n"
		<< "press:\tm/n    \t to   change  the objects diameter\n"
		<< std::endl;

	string filePath;
	if (argc < 2) {
		std::cout << "Please provide a .lin file, if you want a different file\n\n\n";
		filePath = "../../chaos_files/";
		filePath += "Busch_3d_2.lin";
	}
	else
		filePath = argv[1];

	// create window and set up camera
	MyWindow window(&argc, argv, filePath);

	// window.setCamera(MyWindow::CameraType::NONE);
	// window.setCamera(MyWindow::CameraType::STATIC_X_AXIS);
	// window.setCamera(MyWindow::CameraType::STATIC_Y_AXIS);
	// window.setCamera(MyWindow::CameraType::STATIC_Z_AXIS);
	window.setCamera(MyWindow::CameraType::ROTATION); // default value, you propably want to use this

													  // set draw type
													  // default: 0 - draw after each key-input
													  // window.setMaxFPS(60.f);

													  // set first clear color
													  // I don't know why the first 'clear' within function MyWindow::draw does nothing....
	window.clear();

	// start drawing
	// function returns when the "esc-key" is pressed
	return window.startDrawing();
}
