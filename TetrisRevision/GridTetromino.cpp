#include "GridTetromino.h"
#include "Tetromino.h"
#include "Point.h"
#include <vector>

// Make this class extend (inherit from) the Tetromino class.
// Complete each method signature below
// Implement the methods below in a GridTetromino.cpp file.


Point gridLoc;	// the [x,y] location of this tetromino on the grid/gameboard. 
// This loc changes each time the tetromino moves.

// constructor, initialize gridLoc to 0,0
GridTetromino::GridTetromino() 
{
	gridLoc = Point{ 0, 0 };
}

// a getter for the tetromino's location
// - params: none
// - return: a Point (the private member variable gridLoc) 
Point GridTetromino::getGridLoc() const
{
	return gridLoc;
}

// a setter for the tetronimo's location 
// - param 1: int x
// - param 2: int y
// return: nothing
void GridTetromino::setGridLoc(int x, int y)
{
	gridLoc.setX(x);
	gridLoc.setY(y);
}

// a setter for the tetromino's location
// param 1: a Point (the new gridLoc)
// return: nothing
void GridTetromino::setGridLoc(const Point& newGridLoc)
{
	int newX = newGridLoc.getX(), newY = newGridLoc.getY();
	setGridLoc(newX, newY);
}

// transpose the gridLoc of this shape
//	(1,0) represents a move to the right (x+1)
//	(-1,0) represents a move to the left (x-1)
//	(0,1) represents a move down (y+1)
// - param 1: int xOffset, the x offset (distance) to move
// - param 2: int yOffset, the y offset (distance) to move
// - return: nothing
void GridTetromino::move(int xOffset, int yOffset)
{
	int currX = gridLoc.getX(), currY = gridLoc.getY();
	gridLoc.setX(currX + xOffset);
	gridLoc.setY(currY + yOffset);
}

// Build and return a vector of Points to represent our inherited
// blockLocs vector mapped to the gridLoc of this object instance.
// You will need to provide this class access to blockLocs (from the Tetromino class).
// eg: if we have a Point [x,y] in our vector,
// and our gridLoc is [5,6] the mapped Point would be [5+x,6+y].
// params: none:
// return: a vector of Point objects.
std::vector<Point> GridTetromino::getBlockLocsMappedToGrid() const
{
	std::vector<Point> mappedPoints;

	for (Point block : Tetromino::blockLocs)
	{
		int mappedX = block.getX() + gridLoc.getX();
		int mappedY = block.getY() + gridLoc.getY();

		Point p{ mappedX, mappedY };
		mappedPoints.push_back(p);
	}

	return mappedPoints;
}

