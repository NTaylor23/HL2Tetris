#include <iostream>
#include <sstream>
#include "Point.h"

Point::Point() { x = 0, y = 0; }
Point::Point(int x, int y) { this->x = x, this->y = y; }

int Point::getX() const { return x; }
int Point::getY() const { return y; }

void Point::setX(int x) { this->x = x; }
void Point::setY(int y) { this->y = y; }

void Point::setXY(int x, int y)
{
	Point::setX(x);
	Point::setY(y);
}

void Point::swapXY()
{
	int swap = x;
	x = y;
	y = swap;
}

void Point::multiplyX(int factor) { x *= factor; }
void Point::multiplyY(int factor) { y *= factor; }

std::string Point::toString() const {

	std::stringstream state;
	state << "[" << x << "," << y << "]";

	std::string result = state.str();
	return result;
}