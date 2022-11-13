#include <SFML/Graphics.hpp>
#include "TetrisGame.h"
#include "Tetromino.h"
#include <cassert>

const int TetrisGame::BLOCK_WIDTH = 32;
const int TetrisGame::BLOCK_HEIGHT = 32;
const int TetrisGame::MAX_ROWS = 19;
const int TetrisGame::MAX_COLS = 10;
const double TetrisGame::MAX_SECONDS_PER_TICK = 0.75;
const double TetrisGame::MIN_SECONDS_PER_TICK = 0.20;

TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, const Point& gameboardOffset, const Point& nextShapeOffset)
	: window(window), blockSprite(blockSprite), gameboardOffset(gameboardOffset), nextShapeOffset(nextShapeOffset)
{
	reset();

	if (!scoreFont.loadFromFile("fonts/RedOctober.ttf"))
	{
		assert(false && "Missing font: RedOctober.ttf");
	};

	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(18);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(425, 325);
};

sf::RenderWindow& window = window;
sf::Sprite& blockSprite = blockSprite;

void TetrisGame::draw() 
{
	drawGameboard();
	drawTetromino(currentShape, gameboardOffset);
	drawTetromino(nextShape, nextShapeOffset);
	updateScoreDisplay();
	window.draw(scoreText);
}

void TetrisGame::onKeyPressed(sf::Event& event) {

	switch (event.key.code) {
	case sf::Keyboard::Up:
		attemptRotate(currentShape);
		break;
	case sf::Keyboard::Left:
		attemptMove(currentShape, -1, 0);
		break;
	case sf::Keyboard::Right:
		attemptMove(currentShape, 1, 0);
		break;
	case sf::Keyboard::Down:
		attemptMove(currentShape, 0, 1);
		break;
	case sf::Keyboard::Space:
		drop(currentShape);
		lock(currentShape);
	default:
		break;
	}
}

void TetrisGame::processGameLoop(float secondsSinceLastLoop) {
	secondsSinceLastTick += secondsSinceLastLoop;

	if (secondsSinceLastTick > secondsPerTick) {
		secondsSinceLastTick = 0;
		tick();
	}

	if (shapePlacedSinceLastGameLoop) {
		shapePlacedSinceLastGameLoop = false;
		if (spawnNextShape()) {
			pickNextShape();
			int completedRows = board.removeCompletedRows();
			score += 100 * completedRows; //fix this so it's the actual rules.
			determineSecondsPerTick();
		}
		else {
			reset();
		}
	}	
}

void TetrisGame::tick() {
	
	if (!attemptMove(currentShape, 0, 1)) {
		lock(currentShape);
	}
}

void TetrisGame::reset() {
	score = 0;
	determineSecondsPerTick();
	board.empty();
	pickNextShape();
	spawnNextShape();
	pickNextShape();
}

void TetrisGame::pickNextShape() {
	nextShape.setShape(currentShape.getRandomShape());
}


bool TetrisGame::spawnNextShape() { 
	currentShape.setGridLoc(board.getSpawnLoc());
	currentShape.setShape(nextShape.getShape());
	
	return true; 
}


bool TetrisGame::attemptRotate(GridTetromino& shape) { 
	
	GridTetromino temptromino = shape;
	temptromino.rotateClockwise();

	if (isPositionLegal(temptromino)) {
		shape.rotateClockwise();
		return true;
	}

	return false; 
}


bool TetrisGame::attemptMove(GridTetromino& shape, int x, int y) { 

	GridTetromino temptromino = shape;
	temptromino.move(x, y);

	if (isPositionLegal(temptromino)) { 
		shape.move(x, y);
		return true; 
	}

	return false;
}

void TetrisGame::drop(GridTetromino& shape) {
	bool descend = true;

	while (descend) {
		descend = attemptMove(shape, 0, 1);
	}

}

void TetrisGame::lock(const GridTetromino& shape)  {
	std::vector<Point> mappedLocs = shape.getBlockLocsMappedToGrid();
	board.setContent(mappedLocs, static_cast<int>(shape.getColor()));

	shapePlacedSinceLastGameLoop = true;
}


// Graphics methods ==============================================


void TetrisGame::drawBlock(const Point& topLeft, int xOffset, int yOffset, const TetColor& color) 
{
	int colorKey = static_cast<int>(color);
	sf::IntRect rect = sf::IntRect(colorKey * BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_HEIGHT);

	blockSprite.setTextureRect(rect);
	blockSprite.setPosition(topLeft.getX() + (xOffset * BLOCK_WIDTH), topLeft.getY() + (yOffset * BLOCK_HEIGHT));

	window.draw(blockSprite);
}

void TetrisGame::drawGameboard() 
{
	for (int i = 0; i < board.MAX_Y; i++) {
		for (int j = 0; j < board.MAX_X; j++) {

			Point currentPoint{ j, i };
			int currentObject = board.getContent(currentPoint);

			if (currentObject != Gameboard::EMPTY_BLOCK) {
				drawBlock(gameboardOffset, j, i, static_cast<TetColor>(board.getContent(currentPoint)));
			}
		}
	}
}


void TetrisGame::drawTetromino(const GridTetromino& tetromino, const Point& topLeft) 
{
	for (const Point& mappedLoc : tetromino.getBlockLocsMappedToGrid()) {
		drawBlock(gameboardOffset, mappedLoc.getX(), mappedLoc.getY(), tetromino.getColor());
	}
}


void TetrisGame::updateScoreDisplay() {
	auto s = "Score: " + std::to_string(score);
	scoreText.setString(s);
}

// State & gameplay/logic methods ================================

bool TetrisGame::isPositionLegal(const GridTetromino& shape) const { 
	std::vector<Point> mappedLocs = shape.getBlockLocsMappedToGrid();
	bool locsAreEmpty = board.areAllLocsEmpty(mappedLocs);
	bool withinBorders = isWithinBorders(shape);

	return locsAreEmpty && withinBorders;
}

bool TetrisGame::isWithinBorders(const GridTetromino& shape) const { 

	for (Point& p : shape.getBlockLocsMappedToGrid()) {
		
		if ((p.getX() < 0 || p.getX() >= board.MAX_X) || p.getY() >= board.MAX_Y) {
			return false;
		}
	}

	return true; 
}

void TetrisGame::determineSecondsPerTick() {}