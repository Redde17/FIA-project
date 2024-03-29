#include "GameHandler.h"
#include "MazeGenerator.h"
#include "MazeSolver.h"

#include <iostream>

GameHandler::GameHandler(const int& mapSizeX, const int& mapSizeY, const int& gridSize) {
	this->window = new sf::RenderWindow(sf::VideoMode(mapSizeX * gridSize, mapSizeY * gridSize), "MazeAI", sf::Style::Close);
	this->mapSizeX = mapSizeX;
	this->mapSizeY = mapSizeY;
	this->gridSize = gridSize;
}

void GameHandler::generateNewMaze() {
	maze = MazeGenerator::generateMaze(MazeGenerator::RandomDepthFirst, mapSizeX, mapSizeY, *this);
}

void GameHandler::solveMaze() {
	Maze::Node* startNode = &maze.gameMap[1][1];
	Maze::Node* finishNode = &maze.gameMap[mapSizeX - 2][mapSizeY - 2];

	maze.setStartingNode(*startNode);
	maze.setEndingNode(*finishNode);
	MazeSolver::findPath(MazeSolver::AStar, &maze, startNode, finishNode);
}

void GameHandler::drawMaze() {
	window->clear();
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

			if(maze.gameMap[x][y].isPath)
				testTile.setFillColor(sf::Color::Green);
			else if(maze.gameMap[x][y].hasBeenVisited)
				testTile.setFillColor(sf::Color::Cyan);
			else
				testTile.setFillColor(sf::Color::White);


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

	window->display();
}

