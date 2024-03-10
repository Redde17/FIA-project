#include "GameHandler.h"

GameHandler::GameHandler(const int& mapSizeX, const int& mapSizeY, const int& gridSize) {
	this->mapSizeX = mapSizeX;
	this->mapSizeY = mapSizeY;
	this->gridSize = gridSize;
}

void GameHandler::generateNewMaze() {
	maze = MazeGenerator::generateMaze(MazeGenerator::RandomDepthFirst, mapSizeX, mapSizeY);
}

void GameHandler::solveMaze() {
	Maze::Node* startNode = &maze.gameMap[1][1];
	Maze::Node* finishNode = &maze.gameMap[mapSizeX - 2][mapSizeY - 2];

	maze.setStartingNode(*startNode);
	maze.setEndingNode(*finishNode);
	MazeSolver::findPath(MazeSolver::AStar, &maze, startNode, finishNode);
}

void GameHandler::drawMaze(sf::RenderWindow* window) {
	sf::RectangleShape* toBeDrawnTile = NULL;

	sf::RectangleShape testTile(sf::Vector2f(this->gridSize, this->gridSize));
	testTile.setFillColor(sf::Color::White);

	sf::RectangleShape wall(sf::Vector2f(this->gridSize, WALL_THICKNES));
	wall.setFillColor(sf::Color::Black);

	toBeDrawnTile = &testTile;
	for (int x = 1; x < mapSizeX - 1; x++)
	{
		for (int y = 1; y < mapSizeY - 1; y++)
		{
			if (!maze.gameMap[x][y].isExplored)
				continue;

			toBeDrawnTile->setPosition(x * gridSize, y * gridSize);
			window->draw(*toBeDrawnTile);
			
			//define the walls of the tile
			if (maze.gameMap[x][y].walls[maze.LEFT]) {
				//change wall position and rotation
				wall.setPosition(sf::Vector2f((x * gridSize), y * gridSize + gridSize));
				wall.setRotation(270);
				window->draw(wall);
			}

			//define the walls of the tile
			if (maze.gameMap[x][y].walls[maze.UP]) {
				//change wall position and rotation
				wall.setPosition(sf::Vector2f(x * gridSize, (y * gridSize)));
				wall.setRotation(0);
				window->draw(wall);
			}

			//define the walls of the tile
			if (maze.gameMap[x][y].walls[maze.RIGHT]) {
				//change wall position and rotation
				wall.setPosition(sf::Vector2f((x * gridSize) + gridSize, y * gridSize));
				wall.setRotation(90);
				window->draw(wall);
			}

			//define the walls of the tile
			if (maze.gameMap[x][y].walls[maze.DOWN]) {
				//change wall position and rotation
				wall.setPosition(sf::Vector2f((x * gridSize) + gridSize, (y * gridSize) + gridSize));
				wall.setRotation(180);
				window->draw(wall);
			}

		}
	}

	toBeDrawnTile->setScale(0.5f, 0.5f);
	toBeDrawnTile->setPosition(maze.startingNode.x * gridSize, maze.startingNode.y * gridSize);
	toBeDrawnTile->setFillColor(sf::Color::Red);
	window->draw(*toBeDrawnTile);

	toBeDrawnTile->setScale(0.5f, 0.5f);
	toBeDrawnTile->setPosition(maze.endingNode.x * gridSize, maze.endingNode.y * gridSize);
	toBeDrawnTile->setFillColor(sf::Color::Blue);
	window->draw(*toBeDrawnTile);
}

