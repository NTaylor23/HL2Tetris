#include <iostream>
#include <algorithm>
#include <vector>
#include "Tetromino.h"

Tetromino::Tetromino() {
	color = TetColor::RED;
	shape = TetShape::S;
	blockLocs = shapes.at(shape);
}

TetColor Tetromino::getColor() const { return color; }
TetShape Tetromino::getShape() const { return shape; }

TetShape Tetromino::getRandomShape()
{
	int randomShapeNumber = rand() % static_cast<int>(TetShape::COUNT);
	return static_cast<TetShape> (randomShapeNumber);
}

void Tetromino::setShape(TetShape shape) 
{ 
	this->shape = shape;
	blockLocs = shapes.at(shape);
	color = TetColor(static_cast<int> (shape));
}

void Tetromino::printToConsole() const
{
	std::cout << '\n';
	for (int hor = -3; hor <= 3; hor++)
	{
		for (int ver = -3; ver <= 3; ver++)
		{
			bool found = false;
			for (Point point : blockLocs)
			{
				if (hor == point.getX() && ver == point.getY()) { found = true; }
			}
			if (found)
			{
				std::cout << 'X';
			}
			else
			{
				std::cout << '.';
			}
		}
		std::cout << '\n';
	}
}

void Tetromino::rotateClockwise()
{
	if (shape != TetShape::O)
	{
		for (Point& p : blockLocs)
		{
			int currX = p.getX(), currY = p.getY();
			p.setX(currY);
			p.setY(currX * -1);
		}
	}
}