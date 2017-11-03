
#include <windowRasterized.h>
#include <iostream>
#include <random>
#include "Praktikum1-2.h"

cf::WindowRasterized window = cf::WindowRasterized(512, 512);
std::mt19937 generator = std::mt19937(std::random_device().operator()());

int main() {
	window.show();

	window.floodFill(cf::Point(256,256), cf::Color().RED);
	window.show();

	cf::Rect start = cf::Rect(cf::Point(0, 0), cf::Point(511, 511), 1, cf::Color().BLACK);

	splitRectangle(start, 0);
	window.show();

	std::cout << "Press any key to close" << std::endl;
	window.waitKey();
	return 0;
}

void splitRectangle(cf::Rect &start, int depth)
{
	if (depth >= 8) {
		return;
	}

	cf::Point center = (start.point1 + start.point2) / 2;
	float minX = std::min(start.point1.x, start.point2.x);
	float maxX = std::max(start.point1.x, start.point2.x);
	float minY = std::min(start.point1.y, start.point2.y);
	float maxY = std::max(start.point1.y, start.point2.y);

	cf::Point topLeft = cf::Point(minX, minY);
	cf::Point topRight = cf::Point(maxX, minY);
	cf::Point lowerLeft = cf::Point(minX, maxY);
	cf::Point lowerRight = cf::Point(maxX, maxY);

	cf::Rect rectTopLeft = cf::Rect(topLeft, center, 1, start.color);
	cf::Rect rectTopRight = cf::Rect(topRight, center, 1, start.color);
	cf::Rect rectLowerLeft = cf::Rect(lowerLeft, center, 1, start.color);
	cf::Rect rectLowerRight = cf::Rect(lowerRight, center, 1, start.color);

	fillRectangle(rectTopRight);
	splitRectangle(rectTopLeft, depth+1);
	splitRectangle(rectLowerLeft, depth + 1);
	splitRectangle(rectLowerRight, depth + 1);
}

void fillRectangle(cf::Rect &rect)
{
	window.drawRectangle(rect);
	window.floodFill((rect.point1 + rect.point2) / 2, rect.color);
}