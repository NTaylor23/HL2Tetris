// The Gameboard class encapsulates the functionality of the Tetris game board.
//
// The Gameboard class has no concept of what a tetromino is!
// This is intentional.  It helps to keep the gameboard as modular and as simple
// as possible. The gameboard class simply manages a 2D array of content
// (integers). Each integer of content can represent either an empty spot, or a
// color.  Gameboard member functions operate on that content (particularly on
// rows) to facilitate operations that happen on a Tetris gameboard (checking for
// a complete row, filling a row, copying a row, collapsing rows, etc…).
//
// Actual gameplay is handled by a separate class (which makes use of the gameboard).
// We will handle this in a future lab. To get a sense of the bigger picture and how 
// the gameboard is used : a GridTetromino (a special Tetromino that has knowledge of
// its location on a co-ordinate system) will eventually reach a location on the 
// gameboard where it cannot move down any further (because the points that it would
// occupy already contain content).  At this point, the information a Tetromino 
// contains about its blocks (location and color) will be copied to the gameboard grid.
// We use the gameboard to determine what locations are already occupied as a result of 
// previous tetromino being “locked” (where its blocks are copied to the gameboard).
// 
// Displaying the board(via text output) should show empty spots, and areas where blocks
// of color(content) have been copied("locked") onto the board from tetrominos that have
// already been placed(either intentionally or not).
//
// - The game board is represented by a 2D array of integers(the grid).
// - The array contains content(integers) which represent either :
//    - an EMPTY_BLOCK(-1),
//    - a color from the Tetromino::TetColor enum.
// - This 2D array is oriented with [0][0] at the top left and [MAX_Y][MAX_X] at the
//      bottom right.  Why?  It makes the board much easier to draw the grid on the 
//      screen later because this is the same way things are drawn on a screen 
//      co-ordinate system (where pixel 0,0 is considered top left).
// - A 2D array uses row - major ordering by default – which means that the first index
//      is the row and the second is the column.eg: grid[row][col]. However it will be
//      easier for us to think of this grid in terms of XY coordinates. Here’s the 
//      catch… X represents columns and Y represents rows.  (I know this seems counter-
//      intuitive, but to make sense of it - think of pixels on a screen, or XY 
//      co-ordinates on a co-ordinate system)  
// - This means that when we index into a 2D array with an x, y co - ordinate, we are 
//      going to need to reverse what we would probably do naturally : (grid[x][y]), 
//      and index the grid the other way around(grid[y][x]). We will only need to do 
//      this within the gameboard class (since grid is a private member variable). 
//		We are using abstraction to hide the details of implementation inside the class.
//      If we call member functions that are public (eg : setContent(x, y, content))
//      we can treat X and Y as we normally would,
//
//  [expected .cpp size: ~ 225 lines (including method comments)]


#include <vector>
#include "Point.h"
#include "Gameboard.h"
#include <iomanip>
#include <cassert>
#include <algorithm>

// METHODS -------------------------------------------------
// 
// constructor - empty() the grid
Gameboard::Gameboard() 
{
	empty();
};

// fill the board with EMPTY_BLOCK 
//   (iterate through each rowIndex and fillRow() with EMPTY_BLOCK))
// - params: none
// - return: nothing
void Gameboard::empty() 
{
	for (int y = 0; y < Gameboard::MAX_Y; y++)
	{
		fillRow(y, EMPTY_BLOCK);
	}
}

// print the grid contents to the console (for debugging purposes)
//   use std::setw(2) to space the contents out 
//   (google it, you'll need to #include <iomanip>).
//   If the content == EMPTY_BLOCK, print a '.', otherwise print the content.
// - params: none
// - return: nothing
void Gameboard::printToConsole() const 
{
	for (int y = 0; y < MAX_Y; y++)
	{
		for (int x = 0; x < MAX_X; x++)
		{
			int content = grid[y][x];
			if (content == EMPTY_BLOCK)
			{
				std::cout << std::setw(2) << '.';
			}
			else
			{
				std::cout << std::setw(2) << content;
			}	
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

// get the content at a given point 
// assert the point is valid (use isValidPoint())
// - param 1: a Point (the XY of the content weant to retrieve)
// - return: an int, the content from the grid at the specified point 
int Gameboard::getContent(Point pt) const
{
	int x = pt.getY(), y = pt.getX(); //swapped
	return getContent(x, y);
}

// get the content at a given XY location
// assert the point is valid (use isValidPoint())
// - param 1: an int for X (column)
// - param 2: an int for Y (row)
// - return: an int, the content from the grid at the specified XY
int Gameboard::getContent(int x, int y)  const
{
	assert(isValidPoint(x, y) && "Invalid Point!");
	return grid[x][y];
}

// set the content at a given point (ignore invalid points)
// - param 1: a Point
// - param 2: an int representing the content we want to set at this location.
void Gameboard::setContent(Point pt, int content) 
{
	int x = pt.getY(), y = pt.getX();
	setContent(x, y, content);
}

// set the content at an x,y position (ignore invalid points)
// - param 1: an int for X (column)
// - param 2: an int for Y (row)
// - param 3: an int representing the content we want to set at this location	
void Gameboard::setContent(int x, int y, int content) 
{
	grid[y][x] = content;
}

// set the content for a set of points (ignore invalid points)
// - param 1:	a vector of Points representing locations
// - param 2: an int representing the content we want to set.
void Gameboard::setContent(std::vector<Point>& locs, int content) 
{
	for (Point& p : locs)
	{
		int x = p.getY(), y = p.getX();
		assert(isValidPoint(x, y) && "Invalid Point!");
		setContent(y, x, content);
	}
}

// Determine if (valid) all points passed in are empty
// *** IMPORTANT: Assume invalid x,y values can be passed to this method.
// Invalid meaning outside the bounds of the grid.
// * ONLY TEST VALID POINTS (use isValidPoint()). Ignore invalid Points.
// Using invalid points to index into the grid would cause undefined behaviour. 
// - param 1: a vector of Points representing locations to test
// - return: true if the content at ALL VALID points is EMPTY_BLOCK, false otherwise
bool Gameboard::areAllLocsEmpty(std::vector<Point>& locs) const
{
	for (Point& p : locs)
	{
		int x = p.getY(), y = p.getX();
		if (isValidPoint(x, y) && grid[x][y] != EMPTY_BLOCK)
		{
			return false;
		}
	}
	return true;
}

// Remove all completed rows from the board
//   use getCompletedRowIndices() and removeRows() 
// - params: none
// - return: the count of completed rows removed
int Gameboard::removeCompletedRows() 
{
	std::vector<int> completedRows = getCompletedRowIndices();
	removeRows(completedRows);
	return completedRows.size();
}

// A getter for the spawn location
// - params: none
// - returns: a Point, representing our private spawnLoc
Point Gameboard::getSpawnLoc() const  { return spawnLoc; }


// Determine if a given Point is a valid grid location
// - param 1: a Point object
// - return: true if the point is a valid grid location, false otherwise
bool Gameboard::isValidPoint(const Point& pt) const
{
	int x = pt.getY(), y = pt.getX(); //swapped
	return isValidPoint(x, y);
}

// Determine if a given XY is a valid grid location
// - param 1: an int representing x
// - param 2: an int representing y
// - return: true if the x,y is a valid grid location, false otherwise
bool Gameboard::isValidPoint(const int x, const int y) const
{
	/*
	Why doesn't this work? 
	//return (x >= 0 && x < MAX_Y) && (y >= 0 && x < MAX_X);
	*/

	bool validX = x >= 0 && x < MAX_Y;
	bool validY = y >= 0 && y < MAX_X;
	return validX && validY;
}

// return a bool indicating if a given row is full (no EMPTY_BLOCK in the row)
// assert the row index is valid
// - param 1: an int representing the row index we want to test
// - return: bool representing if the row is completed
bool Gameboard::isRowCompleted(const int rowIndex) const
{
	for (int i = 0; i < MAX_X; i++)
	{
		if (grid[rowIndex][i] == EMPTY_BLOCK) return false;
	}

	return true;
}

// fill a given grid row with specified content
// - param 1: an int representing a row index
// - param 2: an int representing content
// - return: nothing
void Gameboard::fillRow(const int rowIndex, const int content) 
{
	for (int i = 0; i < Gameboard::MAX_X; i++)
	{
		Gameboard::grid[rowIndex][i] = content;
	}
}

// scan the board for completed rows.
// Iterate through grid rows and use isRowCompleted(rowIndex)
// - params: none
// - return: a vector of completed row indices (integers).
std::vector<int> Gameboard::getCompletedRowIndices() const
{
	std::vector<int> res;

	for (int i = 0; i < MAX_Y; i++)
	{
		if (isRowCompleted(i)) res.push_back(i);
	}

	return res;
}

// copy a source row's contents into a target row.
// - param 1: an int representing the source row index
// - param 2: an int representing the target row index
// - return: nothing
void Gameboard::copyRowIntoRow(const int sourceRow, const int targetRow) 
{
	for (int i = 0; i < MAX_X; i++)
	{
		grid[targetRow][i] = grid[sourceRow][i];
	} 
}

// In gameplay, when a full row is completed (filled with content)
// it gets "removed".  To be exact, the row itself is not removed
// but the content from the row above it is copied into it.
// This continues all the way up the grid until the first row is copied
// into the second row.  Finally, the first row is filled with EMPTY_BLOCK
// Given a row index:
//   1) Assert the row index is valid
//   2) Starting at rowIndex, copy each row above the removed
//     row "one-row-downwards" in the grid.
//     (loop from y=rowIndex-1 down to 0, and copyRowIntoRow(y, y+1)).
//   3) call fillRow() on the first row (and place EMPTY_BLOCKs in it).
// - param 1: an int representing a row index
// - return: nothing
void Gameboard::removeRow(const int rowIndex) 
{
	assert((rowIndex >= 0 && rowIndex < MAX_Y) && "Row index out of range!");

	for (int y = rowIndex - 1; y >= 0; y--)
	{
		copyRowIntoRow(y, y + 1);
	}

	fillRow(0, EMPTY_BLOCK);
}

// given a vector of row indices, remove them. 
//   Iterate through the vector, call removeRow() on each row index. 
// - param 1: a vector of integers representing row indices we want to remove
// - return: nothing
void Gameboard::removeRows(const std::vector<int>& rowIndices) 
{
	for (int row : rowIndices)
	{
		removeRow(row);
	}
}