#pragma once
#include <SFML/Graphics.hpp>
#include "Maze.h"

#define WALL_THICKNES 1.5f

class MazeGenerator;
class MazeSolver;

class GameHandler
{
	Maze maze;
	int mapSizeX;
	int mapSizeY;
	int gridSize;

public:
	sf::RenderWindow* window;

	GameHandler(const int& mapSizeX, const int& mapSizeY, const int& gridSize);

	void generateNewMaze();
	void solveMaze();

	//draw map
	void drawMaze();
};
