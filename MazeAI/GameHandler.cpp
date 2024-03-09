#include "GameHandler.h"

GameHandler::GameHandler(const int& mapSizeX, const int& mapSizeY, const int& gridSize) {
	this->mapSizeX = mapSizeX;
	this->mapSizeY = mapSizeY;
	this->gridSize = gridSize;
}

void GameHandler::generateNewMaze() {
	maze = new Maze(mapSizeX, mapSizeY);
}

void GameHandler::drawMaze(sf::RenderWindow* window) {
	sf::RectangleShape* toBeDrawnTile = NULL;

	sf::RectangleShape testTile(sf::Vector2f(this->gridSize, this->gridSize));
	testTile.setFillColor(sf::Color(0, 0, 0));

	for (int x = 0; x < mapSizeX; x++)
	{
		for (int y = 0; y < mapSizeY; y++)
		{
			//define the walls of the tile
			window->draw(*toBeDrawnTile);
		}
	}
}

