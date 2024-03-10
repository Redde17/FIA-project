#pragma once
#include "Maze.h"

#include <SFML/Graphics.hpp>

#define WALL_THICKNES 1

class GameHandler
{
	Maze maze;
	int mapSizeX;
	int mapSizeY;
	int gridSize;

public:
	GameHandler(const int& mapSizeX, const int& mapSizeY, const int& gridSize);

	void generateNewMaze();

	//draw map
	void drawMaze(sf::RenderWindow* window);
};
